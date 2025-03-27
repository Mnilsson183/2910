-- SQLite

-- Retrieve the titles of all book which stored in the branch that has the book with
-- the highest no_copies value among all books

SELECT no_of_copies
FROM book_copies
ORDER BY no_of_copies DESC
LIMIT 1;

SELECT book.book_id, title, branch_id, no_of_copies
FROM book_copies bc
JOIN book ON bc.book_id = book.book_id;


SELECT DISTINCT book.title
FROM book_copies bc
JOIN book ON bc.book_id = book.book_id
WHERE bc.branch_id = (
    SELECT branch_id 
    FROM book_copies 
    ORDER BY no_of_copies DESC
    LIMIT 1
);