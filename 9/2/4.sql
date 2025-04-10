DROP VIEW IF EXISTS EMPLOYEES_AFTER_CUTS;

CREATE VIEW EMPLOYEES_AFTER_CUTS (ssn, fname, lname, salary)
AS
SELECT E.ssn, E.fname, E.lname, E.salary
FROM EMPLOYEE E
WHERE salary < 50000
WITH CHECK OPTION;

UPDATE EMPLOYEES_AFTER_CUTS
SET salary = salary * 0.5
WHERE ssn = '123456789';

-- fails
INSERT INTO EMPLOYEES_AFTER_CUTS (ssn, fname, lname, salary)
VALUES ('555555555', 'John', 'Doe', 4000);

-- works
INSERT INTO EMPLOYEES_AFTER_CUTS (ssn, fname, lname, salary)
VALUES ('555555555', 'John', 'Doe', 4000);