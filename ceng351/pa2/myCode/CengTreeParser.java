import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengBook> parseBooksFromFile(String filename)
    {
        ArrayList<CengBook> bookList = new ArrayList<CengBook>();
        try {
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("\\|");
                if (parts.length == 4) {
                    Integer key = Integer.parseInt(parts[0].trim());
                    String title = parts[1].trim();
                    String author = parts[2].trim();
                    String genre = parts[3].trim();
                    bookList.add(new CengBook(key, title, author, genre));
                }
            }
            reader.close();
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.out.println("Error parsing integer: " + e.getMessage());
        }
        return bookList;
    }

    
    public static void startParsingCommandLine(){
    Scanner scanner = new Scanner(System.in);
    while (scanner.hasNextLine()) {
        String line = scanner.nextLine().trim();
        String[] parts = line.split("\\|");
        try {
            switch (parts[0].toLowerCase()) {
                case "quit":
                    scanner.close();
                    return;
                case "add":
                    if (parts.length == 5) {
                        Integer key = Integer.parseInt(parts[1].trim());
                        String title = parts[2].trim();
                        String author = parts[3].trim();
                        String genre = parts[4].trim();
                        CengBookRunner.addBook(new CengBook(key, title, author, genre));
                    }
                    break;
                case "search":
                    if (parts.length == 2) {
                        Integer key = Integer.parseInt(parts[1]);
                        
                        CengBookRunner.searchBook(key);
                    }
                    break;
                case "print":
                    CengBookRunner.printTree();
                    break;
            }
        } catch (NumberFormatException e) {
            System.out.println("Error parsing integer: " + e.getMessage());
        }
    }
    scanner.close();
    }
}
