CREATE TABLE book (

    book_id INTEGER,
    title TEXT,
    publisher_name TEXT,

    PRIMARY KEY (book_id),

    FOREIGN KEY (publisher_name) REFERENCES publisher(name)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,

    CHECK (length(title) < 50)
);
CREATE TABLE book_authors (

    book_id INTEGER,
    author_name TEXT,

    PRIMARY KEY (book_id, author_name),

    FOREIGN KEY (book_id) REFERENCES book(book_id)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,

    CHECK (length(author_name) < 50)
);
CREATE TABLE publisher (

    name TEXT,
    address TEXT,
    phone TEXT,

    PRIMARY KEY (name),

    CHECK (length(name) < 50),
    CHECK (length(address) < 70),
    CHECK (length(phone) < 14)
);
CREATE TABLE book_copies (

    book_id INTEGER,
    branch_id INTEGER,
    no_of_copies INTEGER,

    PRIMARY KEY (book_id, branch_id),

    FOREIGN KEY (book_id) REFERENCES book(book_id)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,
    FOREIGN KEY (branch_id) REFERENCES library_branch(branch_id)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,

    CHECK (no_of_copies > 0)
);
CREATE TABLE book_loans (

    book_id INTEGER,
    branch_id INTEGER,
    card_no INTEGER,
    date_out TEXT,
    due_date TEXT,

    PRIMARY KEY (book_id, branch_id, card_no),

    FOREIGN KEY (book_id) REFERENCES book(book_id)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,
    FOREIGN KEY (branch_id) REFERENCES library_branch(branch_id)
    ON DELETE SET NULL
    ON UPDATE NO ACTION,
    FOREIGN KEY (card_no) REFERENCES borrower(card_no)
    ON DELETE SET NULL
    ON UPDATE NO ACTION

);
CREATE TABLE library_branch (

    branch_id INTEGER,
    branch_name TEXT,
    address TEXT,

    PRIMARY KEY (branch_id),

    CHECK (length(branch_name) < 50),
    CHECK (length(address) < 70)

);
CREATE TABLE borrower (

    card_no INTEGER,
    name TEXT,
    address TEXT,
    phone TEXT,

    PRIMARY KEY (card_no),

    CHECK (length(name) < 50),
    CHECK (length(address) < 70)

);

INSERT INTO book VALUES (1, 'The Great Gatsby', 'Bronny James');
INSERT INTO book VALUES (2, 'The Catcher in the Rye', 'LeBron James');
INSERT INTO book VALUES (3, 'To Kill a Mockingbird', 'Scottie Pippen');
INSERT INTO book VALUES (4, '1984', 'MJ');

INSERT INTO book_authors VALUES (1, 'Bronny James');
INSERT INTO book_authors VALUES (2, 'LeBron James');
INSERT INTO book_authors VALUES (3, 'Scottie Pippen');
INSERT INTO book_authors VALUES (4, 'MJ');

INSERT INTO publisher VALUES ('Bronny James', '1234', '123-456-7890');
INSERT INTO publisher VALUES ('LeBron James', '5678', '098-765-4321');
INSERT INTO publisher VALUES ('Scottie Pippen', '9101', '135-792-4680');
INSERT INTO publisher VALUES ('MJ', '1121', '246-801-3579');

INSERT INTO book_copies VALUES (1, 1, 5);
INSERT INTO book_copies VALUES (2, 2, 3);
INSERT INTO book_copies VALUES (3, 3, 2);
INSERT INTO book_copies VALUES (4, 4, 1);

INSERT INTO library_branch VALUES (1, 'Branch 1', '1234');
INSERT INTO library_branch VALUES (2, 'Branch 2', '5678');
INSERT INTO library_branch VALUES (3, 'Branch 3', '9101');
INSERT INTO library_branch VALUES (4, 'Branch 4', '1121');

INSERT INTO borrower VALUES (1, 'John Doe', '1234', '123-456-7890');
INSERT INTO borrower VALUES (2, 'Jane Doe', '5678', '098-765-4321');
INSERT INTO borrower VALUES (3, 'Jim Doe', '9101', '135-792-4680');
INSERT INTO borrower VALUES (4, 'Jill Doe', '1121', '246-801-3579');

INSERT INTO book_loans VALUES (1, 1, 1, '2021-01-01', '2021-01-15');
INSERT INTO book_loans VALUES (2, 2, 2, '2021-02-01', '2021-02-15');
INSERT INTO book_loans VALUES (3, 3, 3, '2021-03-01', '2021-03-15');
INSERT INTO book_loans VALUES (4, 4, 4, '2021-04-01', '2021-04-15');

PRAGMA foreign_keys = ON;

-- fails check constraints
INSERT INTO book VALUES (5, 'asdfghjklkjhgfdsdfghjklkjhgfdsdfghjkjhgfdsdfghjjhgfdsdfghjhgfdfghjjhgfdfghjhgfddfghgfdfghgdasdasdasdaaaaa', 'MJ');
INSERT INTO book_copies VALUES (1, 1, -5);

-- fails referential integrity
INSERT INTO book VALUES (5, '1983', 'Steven Spielberg');

-- check how foreign key constraints work for on update and on delete
UPDATE publisher SET name = 'Steven Spielberg' WHERE name = 'MJ';

DELETE FROM publisher WHERE name = 'MJ'; -- sets values to null in book table
SELECT * FROM book;
