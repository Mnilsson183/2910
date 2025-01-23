package com.mycompany.app;

import java.util.Scanner;
import java.util.Vector;

public class Tui {

    public static void runTUI(Database database) {
        // clear screen
        System.out.print("\033[H\033[2J");
        String input = "";
        printOptions();
        while (!input.equals("q")) {
            input = promptFor("Enter option: ");

            // clear screen
            System.out.print("\033[H\033[2J");

            Vector<Table> rets = runCommand(input, database);
            if (rets == null) {
                break;
            }
            for (int i = 0; i  < rets.size(); i++) {
                rets.get(i).print();
            }
            printOptions();
        }
    }
    
    private static String promptFor(String prompt) {
        System.out.print(prompt);
        Scanner scanner = new Scanner(System.in);
        while (true) {
            String input = scanner.nextLine();
            if (input.length() > 0) {
                scanner.close();
                return input;
            }
            System.out.println("Invalid input");
            System.out.print(prompt);
        }
    }

    private static Vector<Table> runCommand(String input, Database database) {
        Vector<Table> table = new Vector<Table>();
            
        switch (input) {
            //1) output original list of all students;
            case "a":
                table = Database.a(database.students);
                break;
            //2) output list of all students sorted by name (in alphabetic order and vice versa);
            case "b":
                table = Database.b(database.students);
                break;
            //3) output original list of all courses;
            case "c":
                table = Database.c(database.courses);
                break;
            //4) output list of all courses for the specific semester (specified by the user);
            case "d":
                table = Database.d(promptFor("Enter semester: "), database.courses);
                break;
            //5) output list of all courses for the specific semester sorted by course name (in alphabetic order and vice versa);
            case "e":
                table = Database.e(promptFor("Enter semester: "), database.courses);
                break;
            //6) add new student, add new course, add a grade for the course (user enters student's last name, course code, and grade);
            case "f":
                String lastname = promptFor("Enter student's last name: ");
                String courseCode = promptFor("Enter course code: ");
                Vector<Integer> grades = new Vector<>();
                String gradesString = promptFor("Enter grades separated by commas: ");
                String[] parts = gradesString.split(",");
                for (int i = 0; i < parts.length; i++) {
                    grades.add(Integer.parseInt(parts[i]));
                }
                table = Database.f(lastname, courseCode, grades, database.students, database.courses, database.grades);
                database.writeUpdatedCourses();
                database.writeUpdatedGrades();
                database.writeUpdatedStudents();
                break;
            //7) update student info (user enters student id);
            case "g":
                int id = Integer.parseInt(promptFor("Enter student id: "));
                table = Database.g(id, new Student(id, promptFor("Enter student last name: ")
                    , promptFor("Enter student first name: "), promptFor("Enter student phone number: ")
                    , promptFor("Enter student email: ")), database.students);
                    database.writeUpdatedStudents();
                break;
            //8) search for course by name (successful result: full course info);
            case "h":
                table = Database.h(promptFor("Enter course name: "), database.courses);
                break;
            //9) search for course by code (successful result: full course info);
            case "i":
                table = Database.i(promptFor("Enter course code: "), database.courses);
                break;
            //10) search for student info by last name (successful result: full student info);
            case "j":
                table = Database.j(promptFor("Enter student last name: "), database.students);
                break;
            //11) search for student info by last phone number (successful result: full student info);
            case "k":
                table = Database.k(promptFor("Enter student phone number: "), database.students);
                break;
            //12) output list of all courses taken by student (user enters student lastname) and grades 
            case "l":
                table = Database.l(promptFor("Enter student last name: "), database.courses, database.grades);
                break;
            //13) output list of all courses taken by student (user enters student last name) and grades
            case "m":
                table = Database.m(promptFor("Enter student last name: "), database.courses, database.grades);
                break;
            //14) calculate average grade for a specific student (user enters student last name)
            case "n":
                table = Database.n(promptFor("Enter student last name: "), database.grades, database.courses);
                break;
            //15) calculate the average grade for a specific student for a specific term (user enters student's last name and term)
            case "o":
                table = Database.o(promptFor("Enter student last name: "), promptFor("Enter term: "), database.students, database.grades, database.courses);
                break;
            //16) calculate average grade for specific courses (user enters course name).
            case "p":
                table = Database.p(promptFor("Enter course name: "), database.courses, database.grades);
                break;
            case "q":
                System.out.println("Exiting...");
                return null;

            default:
                System.out.println("Invalid input");
            
        }
        return table;
    }

    private static void printOptions() {
        System.out.println("a) output original list of all students;");
        System.out.println("b) output list of all students sorted by name (in alphabetic order and vice versa);");
        System.out.println("c) output original list of all courses;");
        System.out.println("d) output list of all courses for the specific semester (specified by the user);");
        System.out.println("e) output list of all courses for the specific semester sorted by course name (in alphabetic order and vice versa);");
        System.out.println("f) add new student, add new course, add a grade for the course (user enters student's last name, course code, and grade);");
        System.out.println("g) update student info (user enters student id);");
        System.out.println("h) search for course by name (successful result: full course info);");
        System.out.println("i) search for course by code (successful result: full course info);");
        System.out.println("j) search for student info by last name (successful result: full student info);");
        System.out.println("k) search for student info by last phone number (successful result: full student info);");
        System.out.println("l) output list of all courses taken by student (user enters student lastname) and grades;");
        System.out.println("m) output list of all courses taken by student (user enters student last name) and grades;");
        System.out.println("n) calculate average grade for a specific student (user enters student last name);");
        System.out.println("o) calculate the average grade for a specific student for a specific term (user enters student's last name and term);");
        System.out.println("p) calculate average grade for specific courses (user enters course name).");
        System.out.println("q) exit;");
    }
}
