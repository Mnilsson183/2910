DROP VIEW IF EXISTS WORKS_ON_VIEW;

CREATE VIEW WORKS_ON_VIEW (ssn, lname, dept_name, proj_name, hours, manager_ssn)
AS
SELECT E.ssn, E.lname, D.dname, P.pname, W.phours, D.mgr_ssn
FROM EMPLOYEE E
JOIN WORKS_ON W ON E.ssn = W.essn
JOIN PROJECT P ON W.pno = P.pnumber
JOIN DEPARTMENT D ON P.dnum = D.dnumber

SELECT ssn, lname
FROM WORKS_ON_VIEW
WHERE proj_name IN ('ProductY', 'Reorganization')
AND manager_ssn = '888665555';