package com.mycompany.app;

import java.util.Vector;

public class Course {
    private String name;
    private String semester;
    private int id;

    public Course(String name, String semester, int id) {
        this.name = name;
        this.semester = semester;
        this.id = id;
    }

    public static Course parseLineToCourse(String line) {
        String[] parts = line.split(";");
        return new Course(parts[0], parts[1], Integer.parseInt(parts[2]));
    }

    public String getName() {
        return this.name;
    }

    public String getSemester() {
        return this.semester;
    }

    public int getId() {
        return this.id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSamester(String semester) {
        this.semester = semester;
    }

    public void setId(int id) {
        this.id = id;
    }

    public static int findOpenId(Vector<Course> courses) {
        int id = 1;
        while (true) {
            boolean found = false;
            for (Course course : courses) {
                if (course.getId() == id) {
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

    public static Course findCourseById(Vector<Course> courses, int id) {
        for (Course course : courses) {
            if (course.getId() == id) {
                return course;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        return this.name + ";" + this.semester + ";" + this.id;
    }
}
