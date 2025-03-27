-- SQLite

-- For each branch whose average no_copies value is more than 50, retrieve the
-- branch name and the number of copies stores in that branch.

SELECT bc.branch_id, br.branch_name, AVG(bc.no_of_copies) AS avg_copies
FROM book_copies bc
JOIN library_branch br ON bc.branch_id = br.branch_id
GROUP BY bc.branch_id, br.branch_name;


SELECT bc.branch_id, br.branch_name, SUM(bc.no_of_copies) AS total_copies
FROM book_copies bc
JOIN library_branch br ON bc.branch_id = br.branch_id
GROUP BY bc.branch_id, br.branch_name
HAVING AVG(bc.no_of_copies) > 50;