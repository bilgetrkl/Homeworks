import java.util.ArrayList;
import java.util.List;

public class Viewer implements Observer // viewer class
{
    private List <Non_playable> viewlist; // list of non-playable media
    private int current_index; 
    private final Non_playable no_item; // no item case

    public Viewer () // constructor
    {
        viewlist = new ArrayList <> (); // initializes list
        current_index = 0;

        no_item = new No_item (); // no item case
        viewlist.add (no_item);
    }
    @Override
    public void update (List <Media> media) // updates viewer
    {
        viewlist.clear (); // clears list   
        viewlist.add (no_item); // adds no item case

        for (Media obj : media) 
        {
            if (obj instanceof Image || obj instanceof Text) // checks if media is image or text
            {
                viewlist.add ((Non_playable) obj); // adds media to list
            }
        }
        if (current_index >= viewlist.size ())  // checks if current index is out of bounds
        {
            current_index = 0;
        }
    }
    @Override
    public void clear_list () // clears the list
    {
        viewlist.clear ();
        viewlist.add (no_item);
        current_index = 0;
    }
    public void show_list () // displays list
    {
        if (viewlist.size () <= 1) // checks if list is empty
        {
            System.out.println ("No media to view");
            System.out.print ("\n");
            return;
        }
        for (Non_playable obj : viewlist) 
        {
            obj.info (); // displays information
        }
    }
    public Non_playable currently_viewing () // returns currently viewing media
    {
        if (viewlist.size () <= 1) // checks if list is empty
        {
            System.out.println ("No media to view");
            System.out.print ("\n");
            return no_item;
        }
        return viewlist.get (current_index); // returns media
    }
    public void next (String type) // plays next media
    {
        if (viewlist.size () <= 1 || !contains_type (type)) // only no_item in list
        {
            System.out.println ("No media to view");
            System.out.print ("\n");
            return;
        }
        if (type.equalsIgnoreCase ("text")) // checks if type is text
        {
            if (((Media) viewlist.get (current_index)).get_name () == "No Item") 
            {
                current_index++;
            }
            do 
            {
                current_index = (current_index + 1) % viewlist.size (); // increments index
            } while (!(viewlist.get (current_index) instanceof Text)); // continues until media is text
        } 
        else if (type.equalsIgnoreCase ("image")) // checks if type is image
        {
            do 
            {
                current_index = (current_index + 1) % viewlist.size (); // increments index
            } while (!(viewlist.get (current_index) instanceof Image)); // continues until media is image
        }
    }
    public void previous (String type) // plays previous media
    {
        if (viewlist.size () <= 1 || !contains_type (type)) // only no_item in list
        {
            System.out.println ("No media to view");
            System.out.print ("\n");
            return;
        }
        if (type.equalsIgnoreCase ("text")) // checks if type is text
        {
            if (((Media) viewlist.get (current_index)).get_name () == "No Item") 
            {
                current_index--;
            }
            do 
            {
                current_index = (current_index - 1 + viewlist.size ()) % viewlist.size (); // decrements index
            } while (!(viewlist.get (current_index) instanceof Text)); // continues until media is text
        } 
        else if (type.equalsIgnoreCase ("image")) // checks if type is image
        {
            do 
            {
                current_index = (current_index - 1 + viewlist.size ()) % viewlist.size (); // decrements index
            } while (!(viewlist.get (current_index) instanceof Image)); // continues until media is image
        }
    }
    private boolean contains_type (String type) // checks if list contains type
    {
        for (Non_playable item : viewlist) 
        {
            if (item instanceof Media && item.getClass ().getSimpleName ().equalsIgnoreCase (type)) // checks if type is in list
            {
                return true;
            }
        }
        return false;
    }
}

