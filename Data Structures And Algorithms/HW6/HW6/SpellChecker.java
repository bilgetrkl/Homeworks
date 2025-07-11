import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class SpellChecker {
    public static void main(String[] args) throws IOException {
        GTUHashSet<String> dictionary = new GTUHashSet<>();
        BufferedReader reader = new BufferedReader(new FileReader("dictionary.txt"));
        String word;
        
        while ((word = reader.readLine()) != null)  // reads the dictionary file line by line
            dictionary.add(word.trim());
        reader.close();
        Scanner scanner = new Scanner(System.in);
        
        try {
            while (true) {
                System.out.print("Enter a word for search (exit to quit): ");
                String input = scanner.nextLine().trim();
                
                if (input.equalsIgnoreCase("exit")) {
                    System.out.println("Exiting spell checker.");
                    break;
                }
                
                long startTime = System.nanoTime(); // start timing

                if (dictionary.contains(input)) { // checks if the word is in the dictionary
                    System.out.println("Found.");
                } else { // if the word is not in the dictionary
                    System.out.println("Not Found.");
                    System.out.print("Suggestions: ");
                    GTUList<String> suggestions = getSuggestions(input, dictionary); // get suggestions
                    System.out.println(suggestions);
                }
                long endTime = System.nanoTime(); // end timing
                
                System.out.printf("Lookup and suggestion took %.3f ms\n", (endTime - startTime) / 1e6);
            }
        } finally {
            scanner.close();
        }
    }
    
    private static GTUList<String> getSuggestions(String word, GTUHashSet<String> dictionary) { // gets suggestions for a word
        GTUList<String> suggestions = new GTUList<>();
        GTUHashSet<String> uniqueWords = new GTUHashSet<>(); // tracks unique words for fast lookup

        GTUList<String> edits1 = generateEditDistance(word); // edits can have duplicates
        for (String edit : edits1) { 
            if (dictionary.contains(edit)) { // Check if the edit is in the dictionary
                suggestions.add(edit);
                uniqueWords.add(edit);
            }
        }
        
        GTUHashSet<String> processedEdit = new GTUHashSet<>(); // tracks edit1 words already processed
        for (String edit1_word : edits1) { 
            if (processedEdit.add(edit1_word)) { // only process if not already processed

                GTUList<String> edits2 = generateEditDistance(edit1_word); // 
                for (String edit2_word : edits2) { 
                    if (dictionary.contains(edit2_word)) { // checks if the edit2 word is in the dictionary
                        if (!uniqueWords.contains(edit2_word)) { // check if it's unique
                            suggestions.add(edit2_word);
                            uniqueWords.add(edit2_word);
                        }
                    }
                }
            }
        }
        
        return suggestions;
    }
    
    private static GTUList<String> generateEditDistance(String word) { // generates all possible edits for a word
        int L = word.length();
        int deletions = L;
        int transpositions = (L > 1) ? (L - 1) : 0;
        int replacements = L * 25; 
        int insertions = (L + 1) * 26;
        int estimatedSize = deletions + transpositions + replacements + insertions;
        
        GTUList<String> result = new GTUList<>(Math.max(10, estimatedSize));
        StringBuilder sb;

        for (int i = 0; i < L; i++) { // removes one character (deletion)
            sb = new StringBuilder(word);
            sb.deleteCharAt(i);
            result.add(sb.toString());
        }

        if (L > 1) { // swaps adjacent characters (transposition)
            for (int i = 0; i < L - 1; i++) {
                sb = new StringBuilder(word);
                char charI = sb.charAt(i);
                char charIplus1 = sb.charAt(i + 1);
                sb.setCharAt(i, charIplus1);
                sb.setCharAt(i + 1, charI);
                result.add(sb.toString());
            }
        }
        
        for (int i = 0; i < L; i++) { // changes one character to another (replacement)
            sb = new StringBuilder(word);
            for (char c = 'a'; c <= 'z'; c++) {
                if (c != word.charAt(i)) {
                    sb.setCharAt(i, c);
                    result.add(sb.toString());
                }
            }
        }
        
        for (int i = 0; i <= L; i++) { // adds one character (insertion)
            for (char c = 'a'; c <= 'z'; c++) {
                sb = new StringBuilder(word);
                sb.insert(i, c);
                result.add(sb.toString());
            }
        }
        
        return result;
    }
}