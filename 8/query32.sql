SELECT branch_id
FROM book_copies
GROUP BY branch_id
HAVING COUNT(DISTINCT book_id) > 3;

SELECT bc.branch_id, COUNT(*) AS books_c
FROM book_copies bc
WHERE bc.no_of_copies > 5
GROUP BY bc.branch_id;

SELECT bc.branch_id, COUNT(*) AS book_c
FROM book_copies bc
WHERE bc.no_of_copies > 5
AND bc.branch_id IN (
    SELECT branch_id
    FROM book_copies
    GROUP BY branch_id
    HAVING COUNT(DISTINCT book_id) > 3
)
GROUP BY bc.branch_id;
