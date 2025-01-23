package com.mycompany.app;

/**
 * Hello world!
 */
public class App {
    public static void main(String[] args) {
        boolean exampleOutput = false;
        if(args.length > 0 && args[0].equals("--example")) {
            exampleOutput = true;
        }
        Database database = new Database();
        database.init();
        if(exampleOutput) {
            database.runExamploutput();
        } else {
            Tui.runTUI(database);
        }
    }
}
