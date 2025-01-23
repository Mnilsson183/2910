package com.mycompany.app;

import java.util.Vector;

public class Grade {
    private int id;
    private String lastname;
    private String firstname;
    private Vector<Integer> grades = new Vector<>();

    String srcFilename = "grades.csv";

    public Grade(int id, String lastname, String firstname, Vector<Integer> grades) {
        this.id = id;
        this.lastname = lastname;
        this.firstname = firstname;
        this.grades = grades;
    }

    public static Grade parseLineToGrade(String line) {
        String[] parts = line.split(";");
        Vector<Integer> grades = new Vector<>();
        for (int i = 3; i < parts.length; i++) {
            grades.add(Integer.parseInt(parts[i]));
        }
        return new Grade(Integer.parseInt(parts[0]), parts[1], parts[2], grades);
    }

    public int getId() {
        return this.id;
    }

    public String getLastname() {
        return this.lastname;
    }

    public String getFirstname() {
        return this.firstname;
    }

    public Vector<Integer> getGrades() {
        return this.grades;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setLastname(String lastname) {
        this.lastname = lastname;
    }

    public void setFirstname(String firstname) {
        this.firstname = firstname;
    }

    public void setGrades(Vector<Integer> grades) {
        this.grades = grades;
    }

    public static int findOpenId(Vector<Grade> grades) {
        int id = 1;
        while (true) {
            boolean found = false;
            for (Grade grade : grades) {
                if (grade.getId() == id) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                break;
            }
            id++;
        }
        return id;
    }

    public Grade getGradeById(Vector<Grade> grades, int id) {
        for (Grade grade : grades) {
            if (grade.getId() == id) {
                return grade;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        String gradesString = "";
        for (int i = 0; i < this.grades.size(); i++) {
            gradesString += this.grades.get(i);
            if (i < this.grades.size() - 1) {
                gradesString += ";";
            }
        }
        return this.id + ";" + this.lastname + ";" + this.firstname + ";" + gradesString;
    }
}
