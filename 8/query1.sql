-- SQLite

-- Retrieve the names of borrowers who have no loans

SELECT card_no, name, address, phone
FROM borrower
WHERE NOT EXISTS (
    SELECT 1 
    FROM book_loans
    WHERE borrower.card_no = book_loans.card_no
);

SELECT card_no, name, address, phone FROM borrower;

SELECT book_id, branch_id, card_no, date_out, due_date
FROM book_loans;