CS2910 - Assignment 1

This project is a basic database manager built from the lab assignment specifications

## Building
To build the project, run the following command:
```make```

To run the project, run the following command:
```make run```

To generate a sample run of the project run:
```make run-example```

The data structures used in this project are:
Grade {
    int grade;
    String student;
    String course;
}

Student {
    int id;
    String lastname;
    String firstname;
    String phonenumber;
    String email;
}

Grade {
    int id;
    String lastname;
    String firstname;
    Vector<Integer> grades;
}

Database {
    Vector<Grade> grades;
    Vector<Student> students;
    Vector<Course> courses; 
}

Then finally a generic Table class that is used to standardize outputs from the database

Table {
    Vector<String> headers;
    Vector<Vector<String>> rows;
}