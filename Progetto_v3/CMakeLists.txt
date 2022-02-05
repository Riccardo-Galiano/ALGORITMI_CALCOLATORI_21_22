cmake_minimum_required(VERSION 3.15 )
project(Progetto_v3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Progetto_v3 main.cpp Classi/Date.cpp Classi/UniversityMember.cpp Classi/Student.cpp Classi/Professor.cpp Classi/Course.cpp Classi/SpecificYearCourse.cpp Classi/Exam.cpp Classi/StudyCourse.cpp Classi/University.cpp Classi/Classroom.cpp Classi/Parse.hpp Classi/SessionYear.cpp Classi/SessionLog.cpp Classi/ExamDay.cpp Classi/InvalidDbException.cpp Classi/SystemLog.cpp)
