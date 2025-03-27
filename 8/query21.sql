SELECT MAX(no_of_copies) 
FROM book_copies bc
JOIN library_branch lb ON bc.branch_id = lb.branch_id
WHERE lb.branch_name = 'Branch 1';


SELECT book.title, book_copies.no_of_copies
FROM book_copies
JOIN book ON book_copies.book_id = book.book_id;


SELECT book.title, book_copies.no_of_copies
FROM book_copies
JOIN book ON book_copies.book_id = book.book_id
WHERE book_copies.no_of_copies > (
    SELECT MAX(bc.no_of_copies) 
    FROM book_copies bc
    JOIN library_branch lb ON bc.branch_id = lb.branch_id
    WHERE lb.branch_name = 'Branch 1'
);

