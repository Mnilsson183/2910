
-- 1
-- enter your commands here
SELECT name, tot_cred, dept_name
FROM student
WHERE dept_name = "Comp. Sci."
AND tot_cred >= 50 

-- 2
-- enter your commands here
SELECT name, dept_name
FROM student
WHERE dept_name = "Comp. Sci."
OR dept_name = "Elec. Eng."
ORDER BY dept_name, name

-- 3
SELECT name, dept_name, salary
FROM instructor
WHERE dept_name
IN ("Physics", "History", "Finance", "Music")
ORDER BY dept_name

-- 4
SELECT DISTINCT sec_id
FROM section

-- 5
SELECT
	name, 
	(SELECT MAX(salary) FROM instructor) AS max_salary
	WHERE max_salary = salary
	FROM instructor;

SELECT 
	name, 
	salary, 
	(SELECT MAX(salary) FROM instructor) AS max_salary,
	ABS(salary - (SELECT MAX(salary) FROM instructor)) AS dif_salary FROM instructor;

-- 6
SELECT COUNT(DISTINCT course_id) AS n_course_id FROM section;
SELECT DISTINCT course_id, sec_id FROM section;

-- 7
SELECT title, credits, dept_name 
FROM course 
WHERE dept_name = "Comp. Sci."

-- 8
SELECT student.ID, student.name, instructor.name
FROM advisor, instructor, student
WHERE i_id = instructor.ID 
	AND instructor.name = "Katz"
	AND s_id = student.ID

-- 9
SELECT 
	name, 
	salary,
	(salary * 1.25) AS resulting_salary,
	(salary * 0.25) AS difference_salary, -- to match with the query in assignment
	instructor.dept_name
FROM instructor, department
WHERE department.dept_name = instructor.dept_name
AND department.dept_name = "Comp. Sci."

-- 10
SELECT
	takes.ID,
	title,
	takes.grade,
	takes.sec_id,
	takes.semester,
	takes.year
FROM takes, course
WHERE ID = "00128" 
AND takes.course_id = course.course_id