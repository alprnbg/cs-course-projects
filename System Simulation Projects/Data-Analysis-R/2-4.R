library("xlsx")
std_error <- function(x) sd(x)/sqrt(length(x))
pexp_interval <- function(a,b,rate) pexp(b, rate) - pexp(a, rate)


data <- read.xlsx("input.xls", sheetIndex = 1)

day1 <- data[['Day.1']]
day2 <- data[['Day.2']]

#               1           2                3         4             5          6          7          8
day1_stats <- c(mean(day1), std_error(day1), sd(day1), median(day1), min(day1), max(day1), sum(day1), length(day1))
day2_stats <- c(mean(day2), std_error(day2), sd(day2), median(day2), min(day2), max(day2), sum(day2), length(day2))

day1_hist <- hist(day1, breaks=seq(0,ceiling(day1_stats[6])+10,by=10))
day2_hist <- hist(day2, breaks=seq(0,ceiling(day2_stats[6])+10,by=10))

print("Day 1 Stats")
print(day1_stats)

print("Day 2 Stats")
print(day2_stats)

print("#######################################################")

print("Chi-Square Test for Day 1")

observed <- day1_hist$counts
expected <- vector(,length(observed))

index <- 1
for (i in seq(10, length(observed)*10,10)){
    expected[index] <- pexp_interval(i-10, i, 1/day1_stats[1])
    index <- index+1
}
expected <- expected*day1_stats[8]

chisq_stat <- sum((observed-expected)**2/expected)
p_value = 1-pchisq(chisq_stat, df=length(expected)-2)

paste0("Chi-square statistics for day 1: ", chisq_stat)
paste0("P-value: ", p_value)

print("#######################################################")

print("Chi-Square Test for Day 1 With Built-in Function")

observed <- day1_hist$counts
prob.exp <- dexp(seq(0, length(observed)*10-1,10), rate=1/day1_stats[1])
# Get the statisctic only, because default dof used in calculation of p-value is wrong
chisq_stat <- chisq.test(observed, p=prob.exp, rescale.p = TRUE)$statistic 
p_value = 1-pchisq(chisq_stat, df=length(expected)-2)

paste0("Chi-square statistics for day 1: ", chisq_stat)
paste0("P-value: ", p_value)

print("#######################################################")

print("Chi-Square Test for Day 2")

observed <- day2_hist$counts
expected <- vector(,length(observed))

index <- 1
for (i in seq(10, length(observed)*10,10)){
    expected[index] <- pexp_interval(i-10, i, 1/day2_stats[1])
    index <- index+1
}
expected <- expected*day2_stats[8]

chisq_stat <- sum((observed-expected)**2/expected)
p_value = 1-pchisq(chisq_stat, df=length(expected)-2)

paste0("Chi-square statistics for day 2: ", chisq_stat)
paste0("P-value: ", p_value)

print("#######################################################")

print("Chi-Square Test for Day 2 With Built-in Function")

observed <- day2_hist$counts
prob.exp <- dexp(seq(0, length(observed)*10-1,10), rate=1/day2_stats[1])
# Get the statisctic only, because default dof used in calculation of p-value is wrong
chisq_stat <- chisq.test(observed, p=prob.exp, rescale.p = TRUE)$statistic 
p_value = 1-pchisq(chisq_stat, df=length(expected)-2)

paste0("Chi-square statistics for day 2: ", chisq_stat)
paste0("P-value: ", p_value)



