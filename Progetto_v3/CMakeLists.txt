cmake_minimum_required(VERSION 3.15 )
project(Progetto_v3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Progetto_v3 main.cpp Date.cpp UniversityMember.cpp Student.cpp Professor.cpp Course.cpp SpecificYearCourse.cpp Exam.cpp StudyCourse.cpp University.cpp Classroom.cpp Parse.hpp SessionYear.cpp ExamDay.cpp SessionLog.cpp InvalidDbException.cpp SystemLog.cpp)
