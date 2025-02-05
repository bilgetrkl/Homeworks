import java.util.ArrayList;
import java.util.List;

public class Player implements Observer // player class
{
    private List <Playable> playlist; // list of playable media
    private int current_index;
    private final No_item no_item; // no item case

    public Player () // constructor
    {
        playlist = new ArrayList <> (); // initializes list
        current_index = 0;

        no_item = new No_item (); // no item case
        playlist.add (no_item);
    }
    @Override
    public void update (List <Media> media) // updates player
    {
        playlist.clear (); // clears list
        playlist.add (no_item); // adds no item case
        for (Media obj : media) 
        {
            if (obj instanceof Audio || obj instanceof Video) // checks if media is audio or video
            {
                playlist.add ((Playable) obj);  // adds media to list
            }
        }
        if (current_index >= playlist.size ()) // checks if current index is out of bounds
        {
            current_index = 0;
        }
    }
    @Override
    public void clear_list () // clears the list
    {
        playlist.clear ();
        playlist.add (no_item);
        current_index = 0;
    }
    public void show_list () // displays list
    {
        if (playlist.size () <= 1) // checks if list is empty
        {
            System.out.println ("No media to play");
            System.out.print ("\n");
            return;
        }
        for (Playable obj : playlist) 
        {
            obj.info (); // displays information
        }
    }
    public Playable currently_playing () // returns currently playing media
    {
        if (playlist.size () <= 1) // checks if list is empty
        {
            System.out.println ("No media to play");
            System.out.print ("\n");
            return no_item;
        }
        return playlist.get (current_index); // returns media
    }
    public void next (String type) // plays next media
    {
        if (playlist.size () <= 1 || !contains_type (type)) // only no_item in list
        {
            System.out.println ("No media to play");
            System.out.print ("\n");
            return;
        }
        if (type.equalsIgnoreCase ("audio")) // checks if type is audio
        {                
            if (((Media) playlist.get (current_index)).get_name () == "No Item") 
            {
                current_index++;
            }
            do 
            {
                current_index = (current_index + 1) % playlist.size (); // increments index
            } while (!(playlist.get (current_index) instanceof Audio)); // continues until media is audio
        } 
        else if (type.equalsIgnoreCase ("video")) // checks if type is video
        {
            do 
            {
                current_index = (current_index + 1) % playlist.size (); // increments index
            } while (!(playlist.get (current_index) instanceof Video)); // continues until  media is video
        }
    }
    public void previous (String type) // plays previous media
    {
        if (playlist.size () <= 1 || !contains_type (type)) // only no_item in list
        {
            System.out.println ("No media to play");
            System.out.print ("\n");
            return;
        }
        if (type.equalsIgnoreCase ("audio")) // checks if type is audio
        {   
            if (((Media) playlist.get (current_index)).get_name () == "No Item") 
            {
                current_index--;
            }
            do 
            {
                
                current_index = (current_index - 1 + playlist.size ()) % playlist.size (); // decrements index
            } while (!(playlist.get (current_index) instanceof Audio)); // continues until media is audio
        } 
        else if (type.equalsIgnoreCase ("video")) // checks if type is video
        {
            do 
            {
                current_index = (current_index - 1 + playlist.size ()) % playlist.size (); // decrements index
            } while (!(playlist.get (current_index) instanceof Video)); // continues until media is video
        }
    }
    private boolean contains_type (String type) // checks if list contains type
    {
        for (Playable item : playlist) 
        {
            if (item instanceof Media && item.getClass ().getSimpleName ().equalsIgnoreCase (type)) // checks if type is in list
            {
                return true;
            }
        }
        return false;
    }
}
