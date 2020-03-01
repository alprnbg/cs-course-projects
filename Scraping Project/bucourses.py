#!/usr/bin/env python

import pandas as pd
import numpy as np
import time
from io import StringIO
import sys


def get_semester_column_name(semester):
    '''
    This function gets a semester and converts it to a string(semester name) such as (2017, 2018, 1) to 2017-Fall.
    '''
    if semester[2] == 1:
        return str(semester[0])+"-Fall"
    elif semester[2] == 2:
        return str(semester[1])+"-Spring"
    else:
        return str(semester[1])+"-Summer"


def get_semester_name(str):
    '''
    This function gets a semester name and converts it to a semester such as 2017-Fall to (2017, 2018, 1).
    '''
    year = int(str[:4]) #year part of semester name
    term = str[5:] #term part of semester name
    if term == "Fall":
        return (year, year+1, 1)
    elif term == "Spring":
        return (year-1, year, 2)
    elif term == "Summer":
        return (year-1, year, 3)
    else:
        print("False parameter")


def get_course_name(str):
    '''
    This function takes a course and department name as parameter, convert it to human readable form.
    '''
    str += '   '
    new = [] #we will put the new string char by char to an array
    for i in range(0,len(str)-3): #get rid of +, %26, %3a, %2c
        if str[i] == '+':
            new.append(' ')
        elif str[i:i+3] == '%2c':
            new.append(',')
        elif str[i:i+3] == '%3a':
            new.append(':')
        elif str[i:i+3] == '%26':
            new.append('&')
        elif str[i] == ' ':
            continue
        elif str[i] == '2':
            continue
        elif str[i] == '6':
            continue
        elif str[i] == '3':
            continue
        elif str[i] == 'a':
            continue
        elif str[i] == 'c':
            continue

        else:
            new.append(str[i])
    return "".join(new) #convert the array to a string and return it

def course_statistics(dept, semester):
    '''
    This function calculates the number of graduate courses, undergraduate courses and distinct instructors of a department in a semester.
    '''

    this_course_list = all_courses[dept] #list of all courses in that department
    answer = [0, 0, 0]
    instr_list = []
    for course_key in this_course_list:
        grad, undergrad = 0, 0
        try:
            if int(course_key[-3]) > 4: #check key of the course, for example for CMPE150 it will check '1' and undegrad will be 1
                grad = 1
            else:
                undergrad = 1
        except ValueError or TypeError: #if there is not a number there, course is undergraduate
                undergrad = 1
        temp_list = this_course_list[course_key]
        this_semester_dict = temp_list[2] #there is a dictionary in 2nd element of the array and it is like: {(2017, 2018, 1): [Cam Ozturan, Cem Say]}
        if semester in this_semester_dict: #if that dictionary contains the semester
            if undergrad == 1:
                answer[0] += 1
            else:
                answer[1] += 1
            instr_list += this_semester_dict[semester] #add the instructors to the instructor list
    instr_num = len(set(instr_list)) #finds the number of distinc instructors
    answer[2] = instr_num
    return answer

def course_statistics_total(dept):
    """
    This function takes all_courses dictionary and calculates number of grad and undergrad courses for given department.
    """
    this_course_list = all_courses[dept] #list of all courses in that department
    grad = 0
    undergrad = 0
    instr_num = 0
    for course_key in this_course_list:
        try:
            if int(course_key[-3]) > 4: #check key of the course, for example for CMPE150 it will check '1' and undegrad will be 1
                grad += 1
            else:
                undergrad += 1
        except ValueError or TypeError: #if there is not a number there, course is undergraduate
            undergrad += 1
    answer = [undergrad, grad]

    return answer

def total_offerings_func(dept):
    """
    This function takes all_courses dictionary and calculates grad, undergrad and distinct instructors for a department for total offerings part.
    """
    this_course_list = all_courses[dept] #list of all courses in that department
    answer = [0, 0, 0]
    instr_list = []
    for course_key in this_course_list: #for each course
        grad, undergrad = 0, 0
        try:
            if int(course_key[-3]) > 4: #check key of the course, for example for CMPE150 it will check '1' and undegrad will be 1
                grad = 1
            else:
                undergrad = 1
        except ValueError or TypeError: #if there is not a number there, course is undergraduate
            undergrad = 1
        temp_list = this_course_list[course_key]
        this_semester_dict = temp_list[2] #there is a dictionary in 2nd element of the array that contains semesters and instructors, it is like: {(2017, 2018, 1): [Cam Ozturan, Cem Say]}
        course_num = len(this_semester_dict.keys()) #how many times the course is opened in different semesters
        if grad == 1:
            answer[1] += course_num
        else:
            answer[0] += course_num

        for semester in this_course_list[course_key][2]: #for the number of distinc instructors
            instr_list += this_course_list[course_key][2][semester] #add all to the list
        instr_num = len(set(instr_list)) #calculate the distincts
        answer[2] = instr_num


    return answer




#list of all department names
dept_list = [("ASIA", "ASIAN+STUDIES"),
             ("ASIA", "ASIAN+STUDIES+WITH+THESIS"),
             ("ATA", "ATATURK+INSTITUTE+FOR+MODERN+TURKISH+HISTORY"),
             ("AUTO", "AUTOMOTIVE+ENGINEERING"),
             ("BM", "BIOMEDICAL+ENGINEERING"),
             ("BIS", "BUSINESS+INFORMATION+SYSTEMS"),
             ("CHE", "CHEMICAL+ENGINEERING"),
             ("CHEM", "CHEMISTRY"),
             ("CE", "CIVIL+ENGINEERING"),
             ("COGS", "COGNITIVE+SCIENCE"),
             ("CSE", "COMPUTATIONAL+SCIENCE+%26+ENGINEERING"),
             ("CET", "COMPUTER+EDUCATION+%26+EDUCATIONAL+TECHNOLOGY"),
             ("CMPE", "COMPUTER+ENGINEERING"),
             ("INT", "CONFERENCE+INTERPRETING"),
             ("CEM", "CONSTRUCTION+ENGINEERING+AND+MANAGEMENT"),
             ("CCS", "CRITICAL+AND+CULTURAL+STUDIES"),
             ("EQE", "EARTHQUAKE+ENGINEERING"),
             ("EC", "ECONOMICS"),
             ("EF", "ECONOMICS+AND+FINANCE"),
             ("ED", "EDUCATIONAL+SCIENCES"),
             ("CET", "EDUCATIONAL+TECHNOLOGY"),
             ("EE", "ELECTRICAL+%26+ELECTRONICS+ENGINEERING"),
             ("ETM", "ENGINEERING+AND+TECHNOLOGY+MANAGEMENT"),
             ("ENV", "ENVIRONMENTAL+SCIENCES"),
             ("ENVT", "ENVIRONMENTAL+TECHNOLOGY"),
             ("XMB", "EXECUTIVE+MBA"),
             ("FE", "FINANCIAL+ENGINEERING"),
             ("PA", "FINE+ARTS"),
             ("FLED", "FOREIGN+LANGUAGE+EDUCATION"),
             ("GED", "GEODESY"),
             ("GPH", "GEOPHYSICS"),
             ("GUID", "GUIDANCE+%26+PSYCHOLOGICAL+COUNSELING"),
             ("HIST", "HISTORY"),
             ("HUM", "HUMANITIES+COURSES+COORDINATOR"),
             ("IE", "INDUSTRIAL+ENGINEERING"),
             ("INCT", "INTERNATIONAL+COMPETITION+AND+TRADE"),
             ("MIR", "INTERNATIONAL+RELATIONS%3aTURKEY%2cEUROPE+AND+THE+MIDDLE+EAST"),
             ("MIR", "INTERNATIONAL+RELATIONS%3aTURKEY%2cEUROPE+AND+THE+MIDDLE+EAST+WITH+THESIS"),
             ("INTT", "INTERNATIONAL+TRADE"),
             ("INTT", "INTERNATIONAL+TRADE+MANAGEMENT"),
             ("LS", "LEARNING+SCIENCES"),
             ("LING", "LINGUISTICS"),
             ("AD", "MANAGEMENT"),
             ("MIS", "MANAGEMENT+INFORMATION+SYSTEMS"),
             ("MATH", "MATHEMATICS"),
             ("SCED", "MATHEMATICS+AND+SCIENCE+EDUCATION"),
             ("ME", "MECHANICAL+ENGINEERING"),
             ("MECA", "MECHATRONICS+ENGINEERING"),
             ("BIO", "MOLECULAR+BIOLOGY+%26+GENETICS"),
             ("PHIL", "PHILOSOPHY"),
             ("PE", "PHYSICAL+EDUCATION"),
             ("PHYS", "PHYSICS"),
             ("POLS", "POLITICAL+SCIENCE%26INTERNATIONAL+RELATIONS"),
             ("PRED", "PRIMARY+EDUCATION"),
             ("PSY", "PSYCHOLOGY"),
             ("YADYOK", "SCHOOL+OF+FOREIGN+LANGUAGES"),
             ("SCED", "SECONDARY+SCHOOL+SCIENCE+AND+MATHEMATICS+EDUCATION"),
             ("SPL", "SOCIAL+POLICY+WITH+THESIS"),
             ("SOC", "SOCIOLOGY"), ("SWE", "SOFTWARE+ENGINEERING"),
             ("SWE", "SOFTWARE+ENGINEERING+WITH+THESIS"),
             ("TRM", "SUSTAINABLE+TOURISM+MANAGEMENT"),
             ("SCO", "SYSTEMS+%26+CONTROL+ENGINEERING"),
             ("TRM", "TOURISM+ADMINISTRATION"),
             ("WTR", "TRANSLATION"),
             ("TR", "TRANSLATION+AND+INTERPRETING+STUDIES"),
             ("TK", "TURKISH+COURSES+COORDINATOR"),
             ("LL", "WESTERN+LANGUAGES+%26+LITERATURES")]
start = time.time()

n = 1998  # create the list for years and semesters
semesters = []
for i in range(21): #for each year
    for a in range(1, 4): #for each semester
        semesters += [(n, n + 1, a)] #such as (2014, 2015, 1)
    n += 1
semesters = semesters[:-1] #get rid of the last one, summer semester of 2018-2019


semester_names = []


semester_1 = get_semester_name(sys.argv[1]) #input 1, first semester
semester_2 = get_semester_name(sys.argv[2]) #input 2, second semester

for semester_index in range(len(semesters)): #this part gives the indexes of semester[] list that we will use
    if semesters[semester_index] == semester_1: #first index
        first_index = semester_index
    if semesters[semester_index] == semester_2: #second index
        second_index = semester_index

semesters = semesters[first_index:second_index+1] #we will use the semesters between two indexes

for s in semesters:
    semester_names.append(get_semester_column_name(s)) #create the list of semester names for the semesters that we will use


all_courses = {} # Information about courses will be stored in this dictionary.

for semester in semesters:
    for dept in dept_list:
        # get the html table
        url = "https://registration.boun.edu.tr/scripts/sch.asp?donem={}/{}-{}&kisaadi={}&bolum={}".format(semester[0],
                                                                                                           semester[1],
                                                                                                           semester[2],
                                                                                                           dept[0],
                                                                                                           dept[1])

        try :
            data = pd.read_html(url)[3].iloc[1:,:]
        except ValueError:    # If table does not exist
            continue

        # Store the department name then iterate over the courses in that department and semester.
        if dept not in all_courses:
            all_courses[dept] = {}
        for index, row in data.iterrows():    # Iterate over the rows of the dataframe read from html source code.
            try:
                course_code = row[0][:-3]     # Delete the section part. CMPE150.01 -> CMPE150

                # If the course is already added to the dictionary, just update the info. Otherwise, add that course to the dictionary.
                if course_code in all_courses[dept]:
                    if semester not in all_courses[dept][course_code][2]:
                        all_courses[dept][course_code][2][semester] = [row[5]]
                        all_courses[dept][course_code][3]["total_offering"] += 1
                    else:
                        all_courses[dept][course_code][2][semester].append(row[5])
                else:
                    # Store the course info in the order of department name, course name, instructors by semesters and number of total offering.
                    all_courses[dept][course_code] = [dept, row[2], {semester: [row[5]]}, {"total_offering":1}]
            except TypeError:       # If the course code is NaN, which means the row indicates Lab or Ps.
                continue

# Sort departments by its short names.
sorted_dept = sorted(all_courses.keys())


df = pd.DataFrame(columns=["Department/Program", "Course Code", "Course Name"])

for semester in semester_names:
    df[semester] = []
df["Total Offerings"] = []

for dept in sorted_dept:
    current_course_list = all_courses[dept]
    sorted_courses = sorted(current_course_list.keys())

    # get the total offerings info for all semester (info located in the coursecode column).
    total_offering_list = course_statistics_total(dept)
    total_offering_str = "U"+str(total_offering_list[0])+" G"+str(total_offering_list[1])

    # create the new row that will be appended later.
    new_row = {"Department/Program":get_course_name(dept[0]) + " (" +get_course_name(dept[1])+")" ,"Course Code":total_offering_str,"Course Name":""}

    # for each semester, obtain the necessary information for the department.
    for semester in semesters:
        semester_name = get_semester_column_name(semester)
        semester_offering_list = course_statistics(dept, semester)
        semester_offering_str = "U"+str(semester_offering_list[0])+" G"+str(semester_offering_list[1])+" I"+str(semester_offering_list[2])
        new_row[semester_name] = semester_offering_str

    # get the total offering info for the department and update the new row.
    total_offer_list = total_offerings_func(dept)
    new_row["Total Offerings"] = "U"+str(total_offer_list[0])+" G"+str(total_offer_list[1])+" I"+str(total_offer_list[2])

    # Append the new row to dataframe.
    df = df.append(new_row, ignore_index=True)

    # Iterate over the courses in department and semester, also append a new row that carries course info to the dataframe.
    for course_key in sorted_courses:
        course = current_course_list[course_key]
        new_row = {"Department/Program":"" ,"Course Code":course_key,"Course Name":course[1],"Total Offerings":str(course[3]["total_offering"])+"/"+str(len(set(np.concatenate(list(course[2].values())))))}
        for semester in course[2]:
            semester_name = get_semester_column_name(semester)
            new_row[semester_name] = "x"
        df = df.append(new_row, ignore_index=True)

# in order to print out the table on console.
output = StringIO()

# transform to csv format.
df.to_csv(output, index=False)

# reset the cursor.
output.seek(0)

# print the result
print(output.read())
