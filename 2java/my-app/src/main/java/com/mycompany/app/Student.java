package com.mycompany.app;

import java.util.Vector;

public class Student {
    private int id;
    private String lastname;
    private String firstname;
    private String phonenumber;
    private String email;

    public Student(int id, String lastname, String firstname, String phonenumber, String email) {
        this.id = id;
        this.lastname = lastname;
        this.firstname = firstname;
        this.phonenumber = phonenumber;
        this.email = email;
    }

    public static Student parseLineToStudent(String line) {
        String[] parts = line.split(";");
        return new Student(Integer.parseInt(parts[0]), parts[1], parts[2], parts[3], parts[4]);
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

    public String getPhonenumber() {
        return this.phonenumber;
    }

    public String getEmail() {
        return this.email;
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

    public void setPhonenumber(String phonenumber) {
        this.phonenumber = phonenumber;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public static int findOpenId(Vector<Student> students) {
        int id = 1;
        while (true) {
            boolean found = false;
            for (Student student : students) {
                if (student.getId() == id) {
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

    public Student getStudentById(Vector<Student> students, int id) {
        for (Student student : students) {
            if (student.getId() == id) {
                return student;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        return this.id + ";" + this.lastname + ";" + this.firstname + ";" + this.phonenumber + ";" + this.email;
    }
}