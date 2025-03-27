-- SQLite

SELECT borrower.name, borrower.phone, book.title
FROM borrower
FULL OUTER JOIN book_loans ON borrower.card_no = book_loans.card_no
FULL OUTER JOIN book ON book_loans.book_id = book.book_id;
