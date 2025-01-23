package com.mycompany.app;

import java.util.Vector;

// generic class for a table that can be printed
public class Table {

    Vector<String> headers = new Vector<>();
    Vector<Vector<String>> data = new Vector<>();

    public Table() {
    }

    public Table(Vector<String> headers) {
        this.headers = headers;
    }

    public Table(Vector<String> headers, Vector<Vector<String>> data) {
        this.headers = headers;
        this.data = data;
    }

    public void addRow(Vector<String> row) {
        this.data.add(row);
    }

    public void print() {
        if (this.headers.size() == 0) {
            System.out.println("Table is empty");
            return;
        }
        if (this.data.size() == 0) {
            System.out.println("+-------+\n| Empty |\n+-------+");
            return;
        }

        // find the longest string in each column
        Vector<Integer> columnWidths = new Vector<>();
        for (int i = 0; i < this.headers.size(); i++) {
            columnWidths.add(this.headers.get(i).length());
        }
        for (int i = 0; i < this.data.size(); i++) {
            for (int j = 0; j < this.data.get(i).size(); j++) {
                if (this.data.get(i).get(j).length() > columnWidths.get(j)) {
                    columnWidths.set(j, this.data.get(i).get(j).length());
                }
            }
        }

        // print the line above headers
        System.out.println();
        System.out.print("+");
        for (int i = 0; i < this.headers.size(); i++) {
            System.out.print(" ");
            for (int j = 0; j < columnWidths.get(i); j++) {
            System.out.print("-");
            }
            System.out.print(" +");
        }

        // print the headers
        System.out.print("\n|");
        for (int i = 0; i < this.headers.size(); i++) {
            System.out.print(" " + this.headers.get(i));
            for (int j = 0; j < columnWidths.get(i) - this.headers.get(i).length(); j++) {
            System.out.print(" ");
            }
            System.out.print(" |");
        }

        // print the line below headers
        System.out.print("\n+");
        for (int i = 0; i < this.headers.size(); i++) {
            System.out.print(" ");
            for (int j = 0; j < columnWidths.get(i); j++) {
            System.out.print("-");
            }
            System.out.print(" +");
        }

        // print the data
        for (int i = 0; i < this.data.size(); i++) {
            System.out.print("\n|");
            for (int j = 0; j < this.data.get(i).size(); j++) {
            System.out.print(" " + this.data.get(i).get(j));
            for (int k = 0; k < columnWidths.get(j) - this.data.get(i).get(j).length(); k++) {
                System.out.print(" ");
            }
            System.out.print(" |");
            }
        }

        // print the line below data
        System.out.print("\n+");
        for (int i = 0; i < this.headers.size(); i++) {
            System.out.print(" ");
            for (int j = 0; j < columnWidths.get(i); j++) {
            System.out.print("-");
            }
            System.out.print(" +");
        }
        System.out.println("\n");
    }
}