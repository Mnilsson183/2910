CREATE VIEW DEPT_INFO(Dept_name, n_emps, avg_salary, max_salary, manager_ssn)
AS
SELECT D.dname, COUNT(E.ssn), AVG(E.salary), MAX(E.salary), D.mgr_ssn
FROM DEPARTMENT D
JOIN EMPLOYEE E ON D.dnumber = E.dno
GROUP BY D.dname, D.mgr_ssn;

DROP VIEW IF EXISTS DEPT_INFO;