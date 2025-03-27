-- SQLite

-- Select the names and card numbers of all borrowers who borrow the same book
-- (books) that borrower whose card number = ‘1’borrows

SELECT name, card_no FROM borrower;

SELECT book_id, card_no FROM book_loans WHERE card_no = 1;

SELECT DISTINCT borrower.name, borrower.card_no
FROM borrower
WHERE EXISTS (
    SELECT 1
    FROM book_loans bl1
    WHERE bl1.card_no = borrower.card_no
    AND bl1.book_id IN (
        SELECT bl2.book_id FROM book_loans bl2 WHERE bl2.card_no = 1
    )
) 
AND borrower.card_no != 1;