import simpy
import random
import numpy as np


RANDOM_SEED = 3
random.seed(RANDOM_SEED)


"""
Collect and report statistics on:
+ Utilization of the answering system.
+ Utilization of the operators, ++ 
+ Average Total Waiting Time
+ Maximum Total Waiting Time to Total System Time Ratio,
+ Average number of people waiting to be served by each operator.
+ Average number of customers leaving the system unsatisfied either due to
incorrect routing or due to long waiting times. ++
"""

END_TIME = 0
OPERATOR_UTIL = [0, 0, 0]
UNSATISFIED_PEOPLE = 0
TOTAL_Q_WAITING_TIME = [0, 0, 0]
ANSWERING_UTIL = 0
# --------
NUMBER_OF_CALLS = 5000
CALL_CAPACITY = 100  # Call capacity that auto answering system can handle at the same time.

BREAK_TIME = 3
CALL_INTERARRIVAL_RATE = 6
TAKE_RECORD_MEAN = 5
OPERATOR_BREAKS = [0, 0, 0]  # index 0 is empty all the time.
OPERATORS = [None, None, None]
BREAKS = []
MAX_Q_WAIT_TIME = 10
TOTAL_SYSTEM_TIME = 0

"""
Arrivals:
Exponential interarrival times, mean = 6
The answering system can serve 100 callers simultaneously (100 parallel channels). When
all channels are busy it drops any incoming call without answering.
---
Take records:
exponential , mean = 5
---
Routing:
0.3 - Operator 1
0.7 - Operator 2
After
0.1 chance of misrouting 
A caller that is routed to the wrong operator hangs up immediately. 
-- Unsatisfaction ++ --
---
Waiting Q: 
FCFS 
2 different q's for 2 operators
An arriving customer that has been waiting on hold for 10 minutes hangs up and leaves
the system (reneging). -- Unsatisfaction ++  --
---
Service:
Op1 - LogNormally distributed with mean 12 minutes and standard deviation 6 minutes
Op2 - uniformly distributed between 1 and 7 minutes.
---
Operator Breaks:
When an operator decides to take a break, he/she waits until completing all the customers already waiting
for her/him. -- Until q is empty  ---
If new customers arrive during operators break, they wait in the q
The number of breaks an operator wishes to take during an 8-hour shift is known to be
distributed according to a Poisson distribution with a mean of 8 breaks per shift. -- 1 per hour--
"""


class Call:
    current_call = 0
    total_call_success = 0
    total_call_fail = 0
    total_operator_time = [0, 0]

    def __init__(self, id, env):
        self.id = id
        self.env = env
        self.arrival_t = self.env.now
        self.action = env.process(self.call())

    def call(self):
        global ANSWERING_UTIL, TOTAL_SYSTEM_TIME

        if Call.current_call >= CALL_CAPACITY:
            # Answering system is full, drop the call.
            print("Call{} has been dropped.(Capacity reached)".format(self.id))
            Call.total_call_fail += 1
            TOTAL_SYSTEM_TIME += self.env.now - self.arrival_t
            yield env.process(self.check_end())
            return

        # Take Customer Info (Record)
        Call.current_call += 1
        record_time = random.expovariate(1.0 / TAKE_RECORD_MEAN)
        ANSWERING_UTIL += record_time
        yield env.timeout(record_time)
        Call.current_call -= 1

        operator_random = random.randint(0, 9)
        fault_random = random.randint(0, 9)

        # Routing Fault
        if fault_random == 0:  # %10
            print("Call{} has been dropped.(Wrong routing)".format(self.id))
            Call.total_call_fail += 1
            TOTAL_SYSTEM_TIME += self.env.now - self.arrival_t
            yield env.process(self.check_end())
            return

        yield env.process(self.service(1 if operator_random < 3 else 2))

    def check_end(self):
        if Call.total_call_success + Call.total_call_fail == NUMBER_OF_CALLS:  # termination
            yield env.process(self.end())

    def service(self, operator_id):
        global TOTAL_Q_WAITING_TIME, TOTAL_SYSTEM_TIME
        with OPERATORS[operator_id].request() as req:
            q_arrival = env.now
            yield req
            q_waiting = env.now - q_arrival

            if q_waiting < MAX_Q_WAIT_TIME:
                TOTAL_Q_WAITING_TIME[operator_id] += q_waiting
                print("Call{} -> operator {}".format(self.id, operator_id))
                yield self.env.process(self.serve(operator_id))
            else:
                TOTAL_Q_WAITING_TIME[operator_id] += MAX_Q_WAIT_TIME # can not wait more than max q wait time
                TOTAL_SYSTEM_TIME += q_arrival + MAX_Q_WAIT_TIME - self.arrival_t
                print("Call{} has been dropped.(Waited too much in queue)".format(self.id))
                Call.total_call_fail += 1
                yield env.process(self.check_end())

    def serve(self, operator_id):
        global TOTAL_SYSTEM_TIME
        if operator_id == 1:
            # Calculate mu and sigma for lognormal distribution mean = 12, std = 6
            mean, std = 12, 6
            m, sig = self.get_lognormal_values(mean, std)
            time = random.lognormvariate(sigma=sig, mu=m)
            OPERATOR_UTIL[1] += time
            yield env.timeout(time)

        else:
            time = random.uniform(1, 7)
            OPERATOR_UTIL[2] += time
            yield env.timeout(time)

        print("Call{} has been served by operator {}".format(self.id, operator_id))
        TOTAL_SYSTEM_TIME += self.env.now - self.arrival_t
        Call.total_call_success += 1

        yield env.process(self.check_end())

    def get_lognormal_values(self, mean, std):
        phi = (std ** 2 + mean ** 2) ** 0.5
        mu = np.log(mean ** 2 / phi)
        sigma = (np.log(phi ** 2 / mean ** 2)) ** 0.5
        return mu, sigma

    def end(self):
        global END_TIME, UNSATISFIED_PEOPLE

        END_TIME = self.env.now
        UNSATISFIED_PEOPLE = Call.total_call_fail
        self.action.interrupt()  # causes simulation to end
        yield self.env.timeout(0)


class Break:
    def __init__(self, env, operator_id):
        self.env = env
        self.operator = OPERATORS[operator_id]
        self.operator_id = operator_id
        self.starting_t = self.env.now
        self.action = env.process(self.go_break())

    def go_break(self):
        global BREAKS
        go_back = False
        with self.operator.request() as req:
            yield req  # Wait for access

            if self not in BREAKS:  # breaks from previous shift should not be processed.
                return env.timeout(0)

            if len(self.operator.queue) >= OPERATOR_BREAKS[self.operator_id]:  # >= or > ? Seems like >=
                # go back to the queue
                go_back = True
            else:
                print("Operator{} leaves for a break. Time:{}".format(self.operator_id, env.now))
                OPERATOR_BREAKS[self.operator_id] -= 1
                yield env.timeout(BREAK_TIME)
                print("Operator{} returned from break. Time:{}".format(self.operator_id, env.now))

        if go_back:
            print("Operator{} couldn't leave because queue is not empty".format(self.operator_id))
            yield env.process(self.go_break())

        # remove self from breaks
        for i in range(len(BREAKS)):
            if BREAKS[i] == self:
                # break is done, removing itself from the list
                BREAKS.pop(i)
                break


def call_generator(env):
    for i in range(NUMBER_OF_CALLS):
        yield env.timeout(random.expovariate(1.0 / CALL_INTERARRIVAL_RATE))
        print("Incoming Call{}".format(i + 1))
        Call((i + 1), env)


def break_generator(env, operator_id):
    rate = 60  # 1 in every 60 minutes
    while True:
        yield env.timeout(random.expovariate(1.0 / rate))
        OPERATOR_BREAKS[operator_id] += 1
        print("Operator{} decided to take a break. Time:{}".format(operator_id, env.now))
        BREAKS.append(Break(env, operator_id))


def shift_generator(env):
    global OPERATOR_BREAKS, BREAKS
    shift_duration = 480
    while True:
        print("A NEW SHIFT STARTS")
        OPERATOR_BREAKS = [0, 0, 0]
        BREAKS = []
        yield env.timeout(shift_duration)


def print_statistics():
    print("ANSWERING SYSTEM UTILIZATION RATE: %{}".format(round(ANSWERING_UTIL / (CALL_CAPACITY * END_TIME) * 100, 2)))
    print("OPERATOR 1 UTILIZATION RATE: %{}".format(round(OPERATOR_UTIL[1] / END_TIME * 100, 1)))
    print("OPERATOR 2 UTILIZATION RATE: %{}".format(round(OPERATOR_UTIL[2] / END_TIME * 100, 1)))
    print("AVERAGE QUEUE WAITING TIME: {} minutes".format(round(sum(TOTAL_Q_WAITING_TIME) / NUMBER_OF_CALLS, 2)))
    print("MAXIMUM TOTAL WAITING TIME TO TOTAL SYSTEM TIME RATIO: %{}".format(
        round(sum(TOTAL_Q_WAITING_TIME) / TOTAL_SYSTEM_TIME * 100, 1)))
    print("AVERAGE # OF PEOPLE WAITING FOR OPERATOR1: {}".format(round(TOTAL_Q_WAITING_TIME[1] / END_TIME, 2)))
    print("AVERAGE # OF PEOPLE WAITING FOR OPERATOR2: {}".format(round(TOTAL_Q_WAITING_TIME[2] / END_TIME, 2)))
    print("UNSATISFIED RATE: %{}".format(round(UNSATISFIED_PEOPLE / float(NUMBER_OF_CALLS) * 100, 1)))
    print("TOTAL CALLS:{}  RANDOM SEED:{}".format(NUMBER_OF_CALLS, RANDOM_SEED))


if __name__ == "__main__":
    env = simpy.Environment()
    operator1 = simpy.Resource(env, capacity=1)
    operator2 = simpy.Resource(env, capacity=1)
    OPERATORS[1], OPERATORS[2] = operator1, operator2
    env.process(call_generator(env))
    env.process(break_generator(env, 1))
    env.process(break_generator(env, 2))
    env.process(shift_generator(env))

    try:
        env.run()
    except simpy.Interrupt as interrupt:
        print("SIMULATION ENDED")
        print("END TIME: {}".format(END_TIME))

    print_statistics()
