-- Active: 1711061395799@@127.0.0.1@3306@company
-- -- -- create EMPLOYEE table -- -- --
DROP TABLE IF EXISTS EMPLOYEE;
-- -- -- create EMPLOYEE table -- -- --
create table EMPLOYEE(
fname varchar(20) NOT NULL,
minit varchar(1),
lname varchar(25) NOT NULL,
ssn varchar(9) DEFAULT '000000000',
bdate date,
address varchar(50),
sex varchar(1),
salary int CHECK (salary>0) NOT NULL,
super_ssn varchar(9),
dno int,
PRIMARY KEY(ssn),
FOREIGN KEY (super_ssn) REFERENCES EMPLOYEE(ssn)
ON DELETE SET NULL
ON UPDATE CASCADE
);


DROP TABLE IF EXISTS DEPARTMENT;
 
-- -- -- create DEPARTMENT table -- -- --
create table DEPARTMENT(
dname varchar(25) NOT NULL,
dnumber int,
mgr_ssn varchar(9),
mgr_start_date date,
PRIMARY KEY(dnumber),
FOREIGN KEY (mgr_ssn) REFERENCES EMPLOYEE(ssn)
ON DELETE SET NULL
ON UPDATE CASCADE
);

--your code here--
ALTER TABLE EMPLOYEE
ADD CONSTRAINT fk_dept
FOREIGN KEY (dno)
REFERENCES DEPARTMENT (dnumber)
ON DELETE SET NULL
ON UPDATE CASCADE;



DROP TABLE IF EXISTS DEPT_LOCATIONS;

-- -- -- create DEPT_LOCATIONS table -- -- --
create table DEPT_LOCATIONS(
dnumber int,
dlocation varchar(20) NOT NULL,
PRIMARY KEY(dnumber,dlocation),
FOREIGN KEY (dnumber) REFERENCES DEPARTMENT(dnumber)
ON DELETE CASCADE
ON UPDATE CASCADE
);

DROP TABLE IF EXISTS PROJECT;

-- -- -- create PROJECT table -- -- --
create table PROJECT(
pname varchar(20) NOT NULL,
pnumber int,
plocation varchar(20),
dnum int,
PRIMARY KEY(pnumber),
FOREIGN KEY (dnum) REFERENCES DEPARTMENT(dnumber)
ON DELETE SET NULL
ON UPDATE CASCADE
);

DROP TABLE IF EXISTS WORKS_ON;

-- -- -- create WORKS_ON table -- -- --
create table WORKS_ON(
essn varchar(9),
pno int,
phours float(5,1),
PRIMARY KEY(essn, pno),
FOREIGN KEY (essn) REFERENCES EMPLOYEE(ssn)
ON DELETE NO ACTION
ON UPDATE CASCADE
);

DROP TABLE IF EXISTS DEPENDENT;

-- -- -- create DEPENDENT table -- -- --
create table DEPENDENT(
essn varchar(9),
dependent_name varchar(20) NOT NULL,
sex varchar(1),
bdate date,
relationship varchar(20) NOT NULL,
PRIMARY KEY(essn, dependent_name),
FOREIGN KEY (essn) REFERENCES EMPLOYEE(ssn)
ON DELETE CASCADE
ON UPDATE CASCADE
);