package com.mycompany.app;

import java.io.File;
import java.io.FileWriter;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;
import java.util.Vector;

public class Database {
    Vector<Student> students = new Vector<>();
    Vector<Course> courses = new Vector<>();
    Vector<Grade> grades = new Vector<>();

    String studentsSrc = "students.csv";
    String coursesSrc = "courses.csv";
    String gradesSrc = "grades.csv";
    
    public Database() {
    }

    public Database(String coursesSrc, String studentsSrc, String gradesSrc) {
        this.coursesSrc = coursesSrc;
        this.studentsSrc = studentsSrc;
        this.gradesSrc = gradesSrc;
    }

    public void init() {
        // course parsing
        try {
            File file = new File(this.coursesSrc);
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] parts = line.split(";");
                this.courses.add(new Course(parts[0], parts[1], Integer.parseInt(parts[2])));
            }
            scanner.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }

        // student parsing
        try {
            File file = new File(this.studentsSrc);
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] parts = line.split(";");
                this.students.add(new Student(Integer.parseInt(parts[0]), parts[1], parts[2], parts[3], parts[4]));
            }
            scanner.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }

        // grade parsing
        try {
            File file = new File(this.gradesSrc);
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] parts = line.split(";");
                Vector<Integer> grades = new Vector<>();
                for (int i = 3; i < parts.length; i++) {
                    try {
                        grades.add(Integer.parseInt(parts[i]));
                    } catch (Exception e) {
                        grades.add(Integer.MIN_VALUE);
                    }
                }
                Grade grade = new Grade(Integer.parseInt(parts[0]), parts[1], parts[2], grades);
                this.grades.add(grade);
            }
            scanner.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }

    //1) output original list of all students;
    public static Vector<Table> a(Vector<Student> students) {
        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        for (int i = 0; i < students.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(Integer.toString(students.get(i).getId()));
            row.add(students.get(i).getFirstname());
            row.add(students.get(i).getLastname());
            row.add(students.get(i).getPhonenumber());
            row.add(students.get(i).getEmail());
            table.addRow(row);
        }
        Vector<Table> vector = new Vector<Table>();
        vector.add(table);
        return vector;
    }

    //2) output list of all students sorted by name (in alphabetic order and vice versa);
    public static Vector<Table> b(Vector<Student> students) {
        Vector<Table> vector = new Vector<Table>();

        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        Vector<Vector<String>> rows = new Vector<>();
        for (int i = 0; i < students.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(Integer.toString(students.get(i).getId()));
            row.add(students.get(i).getFirstname());
            row.add(students.get(i).getLastname());
            row.add(students.get(i).getPhonenumber());
            row.add(students.get(i).getEmail());
            rows.add(row);
        }
        Collections.sort(rows, (a, b) -> a.get(2).compareTo(b.get(2)));
        for (int i = 0; i < rows.size(); i++) {
            table.addRow(rows.get(i));
        }
        vector.add(table);

        table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        Collections.reverse(rows);
        for (int i = 0 ; i < rows.size(); i++) {
            table.addRow(rows.get(i));
        }
        vector.add(table);

        return vector;
    }
    
    //3) output original list of all courses;
    public static Vector<Table> c(Vector<Course> courses) {
        Table table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        for (int i = 0; i < courses.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(courses.get(i).getName());
            row.add(courses.get(i).getSemester());
            row.add(Integer.toString(courses.get(i).getId()));
            table.addRow(row);
        }
        Vector<Table> vector = new Vector<Table>();
        vector.add(table);
        return vector;
    }

    //4) output list of all courses for the specific semester (specified by the user);
    public static Vector<Table> d(String semester, Vector<Course> courses) {
        Table table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        for (int i = 0; i < courses.size(); i++) {
            if (courses.get(i).getSemester().equals(semester)) {
                Vector<String> row = new Vector<>();
                row.add(courses.get(i).getName());
                row.add(courses.get(i).getSemester());
                row.add(Integer.toString(courses.get(i).getId()));
                table.addRow(row);
            }
        }
        Vector<Table> vector = new Vector<Table>();
        vector.add(table);
        return vector;
    }

    //5) output list of all courses for the specific semester sorted by course name (in alphabetic order and vice versa);
    public static Vector<Table> e(String semester, Vector<Course> courses) {
        Vector<Table> vector = new Vector<Table>();

        Table table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        Vector<Vector<String>> rows = new Vector<>();
        for (int i = 0; i < courses.size(); i++) {
            if (courses.get(i).getSemester().equals(semester)) {
                Vector<String> row = new Vector<>();
                row.add(courses.get(i).getName());
                row.add(courses.get(i).getSemester());
                row.add(Integer.toString(courses.get(i).getId()));
                rows.add(row);
            }
        }
        Collections.sort(rows, (a, b) -> a.get(0).compareTo(b.get(0)));
        for (int i = 0; i < rows.size(); i++) {
            table.addRow(rows.get(i));
        }
        vector.add(table);

        table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        Collections.reverse(rows);
        for (int i = 0; i < rows.size(); i++) {
            table.addRow(rows.get(i));
        }
        vector.add(table);
        return vector;
    }

    //6) add new student, add new course, add a grade for the course (user enters student's last name, course code, and grade);
    public static Vector<Table> f(String lastname, String courseCode, Vector<Integer> studentGrades, Vector<Student> students, Vector<Course> courses, Vector<Grade> grades) {

        Student student = new Student(Student.findOpenId(students), lastname, "", "", "");
        Course course = new Course(courseCode, "", Course.findOpenId(courses));
        Grade grade = new Grade(Grade.findOpenId(grades), lastname, "", studentGrades);

        students.add(student);
        courses.add(course);
        grades.add(grade);

        Vector<Table> vector = new Vector<Table>();

        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        for (int i = 0; i < students.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(Integer.toString(students.get(i).getId()));
            row.add(students.get(i).getFirstname());
            row.add(students.get(i).getLastname());
            row.add(students.get(i).getPhonenumber());
            row.add(students.get(i).getEmail());
            table.addRow(row);
        }
        vector.add(table);

        table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        for (int i = 0; i < courses.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(courses.get(i).getName());
            row.add(courses.get(i).getSemester());
            row.add(Integer.toString(courses.get(i).getId()));
            table.addRow(row);
        }
        vector.add(table);

        table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Grades")));
        for (int i = 0; i < grades.size(); i++) {
            Vector<String> row = new Vector<>();
            row.add(Integer.toString(grades.get(i).getId()));
            row.add(grades.get(i).getFirstname());
            row.add(grades.get(i).getLastname());

            String gradesString = "";
            for (int j = 0; j < grades.get(i).getGrades().size(); j++) {
                if (grades.get(i).getGrades().get(j) == Integer.MIN_VALUE) {
                    gradesString += "na";
                } else {
                    gradesString += grades.get(i).getGrades().get(j);
                }
                if (j != grades.get(i).getGrades().size() - 1) {
                    gradesString += ", ";
                }
            }
            row.add(gradesString);

            table.addRow(row);
        }
        vector.add(table);

        return vector;
    }

    //7) update student info (user enters student id);
    public static Vector<Table> g(int id, Student student, Vector<Student> students) {
        Vector<Table> vector = new Vector<Table>();
        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        for (int i = 0; i < students.size(); i++) {
            student.setId(id);
            if (students.get(i).getId() == id) {
                students.set(i, student);
            }
            Vector<String> row = new Vector<>();
            row.add(Integer.toString(students.get(i).getId()));
            row.add(students.get(i).getFirstname());
            row.add(students.get(i).getLastname());
            row.add(students.get(i).getPhonenumber());
            row.add(students.get(i).getEmail());
            table.addRow(row);
        }
        vector.add(table);
        return vector;
    }

    //8) search for course by name (successful result: full course info);
    public static Vector<Table> h(String name, Vector<Course> courses) {
        Vector<Table> vector = new Vector<Table>();
        Table table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        for (int i = 0; i < courses.size(); i++) {
            if (courses.get(i).getName().equals(name)) {
                Vector<String> row = new Vector<>();
                row.add(courses.get(i).getName());
                row.add(courses.get(i).getSemester());
                row.add(Integer.toString(courses.get(i).getId()));
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;
    }

    //9) search for course by code (successful result: full course info);
    // exacly the same as h
    public static Vector<Table> i(String name, Vector<Course> courses) {
        Vector<Table> vector = new Vector<Table>();
        Table table = new Table(new Vector<String>(Arrays.asList("Name", "Semester", "ID")));
        for (int i = 0; i < courses.size(); i++) {
            if (courses.get(i).getName().equals(name)) {
                Vector<String> row = new Vector<>();
                row.add(courses.get(i).getName());
                row.add(courses.get(i).getSemester());
                row.add(Integer.toString(courses.get(i).getId()));
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;

    }

    //10) search for student info by last name (successful result: full student info);
    public static Vector<Table> j(String lastname, Vector<Student> students) {
        Vector<Table> vector = new Vector<Table>();
        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        for (int i = 0; i < students.size(); i++) {
            if (students.get(i).getLastname().equals(lastname)) {
                Vector<String> row = new Vector<>();
                row.add(Integer.toString(students.get(i).getId()));
                row.add(students.get(i).getFirstname());
                row.add(students.get(i).getLastname());
                row.add(students.get(i).getPhonenumber());
                row.add(students.get(i).getEmail());
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;
    }

    //11) search for student info by last phone number (successful result: full student info);
    public static Vector<Table> k(String phonenumber, Vector<Student> students) {
        Vector<Table> vector = new Vector<Table>();
        Table table = new Table(new Vector<String>(Arrays.asList("ID", "First Name", "Last Name", "Phone", "Email")));
        for (int i = 0; i < students.size(); i++) {
            if (students.get(i).getPhonenumber().equals(phonenumber)) {
                Vector<String> row = new Vector<>();
                row.add(Integer.toString(students.get(i).getId()));
                row.add(students.get(i).getFirstname());
                row.add(students.get(i).getLastname());
                row.add(students.get(i).getPhonenumber());
                row.add(students.get(i).getEmail());
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;
    }

    //12) output list of all courses taken by student (user enters student lastname) and grades
    public static Vector<Table> l(String lastname, Vector<Course> courses, Vector<Grade> grades) {
        Vector<Table> vector = new Vector<Table>();

        for (int gradeIndex = 0; gradeIndex < grades.size(); gradeIndex++) {
            if (!grades.get(gradeIndex).getLastname().equals(lastname)) continue;
            Table table = new Table(new Vector<String>(Arrays.asList("Course", "Grade", "Last Name")));

            Course courseArray[] = new Course[grades.get(gradeIndex).getGrades().size()];
            for (int i = 0; i < grades.get(gradeIndex).getGrades().size(); i++) {
                for (int j = 0; j < courses.size(); j++) {
                    if (courses.get(j).getId() == i + 1) {
                        courseArray[i] = courses.get(j);
                    }
                }
            }

            for (int i = 0; i < courseArray.length; i++) {
                if (courseArray[i] == null) continue;
                Vector<String> row = new Vector<>();
                row.add(courseArray[i].getName());
                if (grades.get(gradeIndex).getGrades().get(i) == Integer.MIN_VALUE) {
                    row.add("na");
                } else {
                    row.add(Integer.toString(grades.get(gradeIndex).getGrades().get(i)));
                }
                row.add(grades.get(gradeIndex).getLastname());
                table.addRow(row);
            }

            vector.add(table);
        }
        return vector;
    }

    //13) output list of all courses taken by student (user enters student last name) and grades
    public static Vector<Table> m(String lastname, Vector<Course> courses, Vector<Grade> grades) {
        Vector<Table> vector = new Vector<Table>();

        for (int gradeIndex = 0; gradeIndex < grades.size(); gradeIndex++) {
            if (!grades.get(gradeIndex).getLastname().equals(lastname)) continue;
            Table table = new Table(new Vector<String>(Arrays.asList("Course", "Grade", "Last Name")));

            Course courseArray[] = new Course[grades.get(gradeIndex).getGrades().size()];
            for (int i = 0; i < grades.get(gradeIndex).getGrades().size(); i++) {
                for (int j = 0; j < courses.size(); j++) {
                    if (courses.get(j).getId() == i + 1) {
                        courseArray[i] = courses.get(j);
                    }
                }
            }

            for (int i = 0; i < courseArray.length; i++) {
                if (courseArray[i] == null) continue;
                Vector<String> row = new Vector<>();
                row.add(courseArray[i].getName());
                if (grades.get(gradeIndex).getGrades().get(i) == Integer.MIN_VALUE) {
                    row.add("na");
                } else {
                    row.add(Integer.toString(grades.get(gradeIndex).getGrades().get(i)));
                }
                row.add(grades.get(gradeIndex).getLastname());
                table.addRow(row);
            }

            vector.add(table);
        }
        return vector;
    }

    //14) calculate average grade for a specific student (user enters student last name)
    public static Vector<Table> n(String lastname, Vector<Grade> grades, Vector<Course> courses) {
        Vector<Table> vector = new Vector<Table>();
        // course | grade | lastname | average
        // CS2010 | 20    | smith    | 
        // CS2020 | 30    | smith    |
        //        |       |          | 25

        Table table = new Table(new Vector<String>(Arrays.asList("Course", "Grade", "Last Name", "Average")));
        for (int i = 0; i < grades.size(); i++) {
            if (grades.get(i).getLastname().equals(lastname)) {
                Vector<Course> cVector = new Vector<>();
                Grade grade = grades.get(i);
                for (int j = 0; j < grade.getGrades().size(); j++) {
                    for (int k = 0; k < courses.size(); k++) {
                        if (courses.get(k).getId() == j + 1) {
                            cVector.add(courses.get(k));
                        }
                    }
                }

                for (int j = 0; j < cVector.size(); j++) {
                    Vector<String> row = new Vector<>();
                    row.add(cVector.get(j).getName());
                    if (grade.getGrades().get(j) == Integer.MIN_VALUE) {
                        row.add("na");
                    } else {
                        row.add(grade.getGrades().get(j).toString());
                    }
                    row.add(grade.getLastname());
                    row.add("");
                    table.addRow(row);
                }
                int sum = 0;
                int count = 0;
                for (int j = 0; j < grade.getGrades().size(); j++) {
                    if (grade.getGrades().get(j) != Integer.MIN_VALUE) {
                        sum += grade.getGrades().get(j);
                        count++;
                    }
                }
                Vector<String> row = new Vector<>();
                row.add("");
                row.add("");
                row.add("");
                row.add(Integer.toString(sum / count));
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;
    }

    //15) calculate the average grade for a specific student for a specific term (user enters student's last name and term)
    public static Vector<Table> o(String lastname, String term, Vector<Student> students, Vector<Grade> grades, Vector<Course> courses) {
        Vector<Table> vector = new Vector<Table>();

        Table table = new Table(new Vector<String>(Arrays.asList("Course", "Grade", "Last Name", "Average")));
        for (int i = 0; i < grades.size(); i++) {
            if (grades.get(i).getLastname().equals(lastname)) {
                Vector<Course> cVector = new Vector<>();
                Grade grade = grades.get(i);
                for (int j = 0; j < grade.getGrades().size(); j++) {
                    for (int k = 0; k < courses.size(); k++) {
                        if (courses.get(k).getId() == j + 1) {
                            cVector.add(courses.get(k));
                        }
                    }
                }

                for (int j = 0; j < cVector.size(); j++) {
                    if (!cVector.get(j).getSemester().equals(term)) continue;
                    Vector<String> row = new Vector<>();
                    row.add(cVector.get(j).getName());
                    if (grade.getGrades().get(j) == Integer.MIN_VALUE) {
                        row.add("na");
                    } else {
                        row.add(grade.getGrades().get(j).toString());
                    }
                    row.add(grade.getLastname());
                    row.add("");
                    table.addRow(row);
                }
                int sum = 0;
                int count = 0;
                for (int j = 0; j < grade.getGrades().size(); j++) {
                    if (grade.getGrades().get(j) != Integer.MIN_VALUE && cVector.get(j).getSemester().equals(term)) {
                        sum += grade.getGrades().get(j);
                        count++;
                    }
                }
                Vector<String> row = new Vector<>();
                row.add("");
                row.add("");
                row.add("");
                if (count == 0) {
                    row.add("na");
                } else {
                    row.add(Integer.toString(sum / count));
                }
                table.addRow(row);
            }
        }
        vector.add(table);
        return vector;
    }

    //16) calculate average grade for specific courses (user enters course name).
    public static Vector<Table> p(String courseName, Vector<Course> courses, Vector<Grade> grades) {
        Vector<Table> vector = new Vector<Table>();

        for (int courseIndex = 0; courseIndex < courses.size(); courseIndex++) {
            if (!courses.get(courseIndex).getName().equals(courseName)) continue;

            Table table = new Table(new Vector<String>(Arrays.asList("Course", "Grade", "Average")));
            Vector<Integer> courseGrades = new Vector<>();

            for (int gradeIndex = 0; gradeIndex < grades.size(); gradeIndex++) {
                Grade grade = grades.get(gradeIndex);
                if (grade.getGrades().size() <= courseIndex) continue;

                courseGrades.add(grade.getGrades().get(courseIndex));
            }

            for (int i = 0; i < courseGrades.size(); i++) {
                Vector<String> row = new Vector<>();
                row.add(courseName);
                if (courseGrades.get(i) == Integer.MIN_VALUE) {
                    row.add("na");
                } else {
                    row.add(courseGrades.get(i).toString());
                }
                row.add("");
                table.addRow(row);
            }

            int sum = 0;
            int count = 0;
            for (int i = 0; i < courseGrades.size(); i++) {
                if (courseGrades.get(i) != Integer.MIN_VALUE) {
                    sum += courseGrades.get(i);
                    count++;
                }
            }

            Vector<String> row = new Vector<>();
            row.add(courseName);
            row.add("");
            if (count == 0) {
                row.add("na");
            } else {
                row.add(Integer.toString(sum / count));
            }
            table.addRow(row);

            vector.add(table);
        }
        return vector;
    }

    public void runExamploutput() {
        System.out.println("Running example output");
        System.out.println("===========================");

        System.out.println("1) output original list of all students;");
        Vector<Table> table = a(this.students);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("2) output list of all students sorted by name (in alphabetic order and vice versa);");
        table = b(this.students);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("3) output original list of all courses;");
        table = c(this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("4) output list of all courses for the specific semester (specified by the user);");
        System.out.println("Semester = Winter");
        table = d("winter", this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("5) output list of all courses for the specific semester sorted by course name (in alphabetic order and vice versa);");
        System.out.println("Semester = Winter");
        table = e("winter", this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("6) add new student, add new course, add a grade for the course (user enters student's last name, course code, and grade);");
        System.out.println("lastname = jehad, course code = CS2720, grades = 90, 20, na");
        table = f("jehad", "CS2720", new Vector<Integer>(Arrays.asList(90, 20, Integer.MIN_VALUE)), this.students, this.courses, this.grades);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("7) update student info (user enters student id);");
        System.out.println("id = 1, new student info = Lebron James, phonenumber = 123123123, email = lbj@gmail.com");
        table = g(1, new Student(Student.findOpenId(students), "Lebron", "James", "123123123", "lbj@gmail.com"), this.students);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("8) search for course by name (successful result: full course info);");
        System.out.println("Course name = CS2910");
        table = h("CS2910", this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("9) search for course by code (successful result: full course info);");
        System.out.println("Course code = CS2910");
        table = i("CS2910", this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("10) search for student info by last name (successful result: full student info);");
        System.out.println("Student last name = Smith");
        table = j("Smith", this.students);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("11) search for student info by last phone number (successful result: full student info);");
        System.out.println("Student phone number = 4374567812");
        table = k("4374567812", this.students);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("12) output list of all courses taken by student (user enters student lastname) and grades;");
        System.out.println("Student last name = Smith");
        table = l("Smith", this.courses, this.grades);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("13) output list of all courses taken by student (user enters student last name) and grades;");
        System.out.println("Student last name = Smith");
        table = m("Smith", this.courses, this.grades);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("14) calculate average grade for a specific student (user enters student last name);");
        System.out.println("Student last name = Smith");
        table = n("Smith", this.grades, this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("15) calculate the average grade for a specific student for a specific term (user enters student's last name and term);");
        System.out.println("Student last name = Smith, term = winter");
        table = o("Smith", "winter", this.students, this.grades, this.courses);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }
        System.out.println("16) calculate average grade for specific courses (user enters course name).");
        System.out.println("Course name = CS2910");
        table = p("CS2910", this.courses, this.grades);
        for (int i = 0; i < table.size(); i++) {
            table.get(i).print();
        }

        writeUpdatedCourses();
        writeUpdatedGrades();
        writeUpdatedStudents();
    }

    public void writeUpdatedStudents() {
        String studentsFile = "updated_students.csv";
        try {
            File file = new File(studentsFile);
            file.createNewFile();
            String s = "";
            for (int i = 0; i < this.students.size(); i++) {
                Student student = this.students.get(i);
                String line = student.getId() + ";" + student.getLastname() + ";" + student.getFirstname() + ";" + student.getPhonenumber() + ";" + student.getEmail();
                s += line + "\n";
            }
            FileWriter writer = new FileWriter(file);
            writer.write(s);
            writer.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }

    public void writeUpdatedCourses() {
        String coursesFile = "updated_courses.csv";
        try {
            File file = new File(coursesFile);
            file.createNewFile();
            String s = "";
            for (int i = 0; i < this.courses.size(); i++) {
                Course course = this.courses.get(i);
                String line = course.getName() + ";" + course.getSemester() + ";" + course.getId();
                s += line + "\n";
            }
            FileWriter writer = new FileWriter(file);
            writer.write(s);
            writer.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }

    public void writeUpdatedGrades() {
        String gradesFile = "updated_grades.csv";
        try {
            File file = new File(gradesFile);
            file.createNewFile();
            String s = "";
            for (int i = 0; i < this.grades.size(); i++) {
                Grade grade = this.grades.get(i);
                String line = grade.getId() + ";" + grade.getLastname() + ";" + grade.getFirstname();
                for (int j = 0; j < grade.getGrades().size(); j++) {
                    line += ";" + grade.getGrades().get(j);
                }
                s += line + "\n";
            }
            FileWriter writer = new FileWriter(file);
            writer.write(s);
            writer.close();
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
    }

}
