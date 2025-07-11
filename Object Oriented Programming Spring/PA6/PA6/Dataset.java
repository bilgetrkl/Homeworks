import java.util.ArrayList;
import java.util.List;

public class Dataset// dataset class
{
    private List <Observer> observers; // list of observers
    private List <Media> media; // list of media

    public Dataset () // constructor
    {
        observers = new ArrayList <> (); // initializes list of observers
        media = new ArrayList <> (); // initializes list of media
    }
    public void add (Media obj) // adds media to dataset
    {
        media.add (obj); // adds media to list
        notify_observers (); // notifies observers
    }
    public void remove (Playable obj) // removes playable media from dataset
    {
        media.remove ((Media) obj); // removes media from list
        notify_observers (); // notifies observers
    }
    public void remove (Non_playable obj) // removes non-playable media from dataset
    {
        media.remove ((Media) obj); // removes media from list
        notify_observers (); // notifies observers
    }
    public void remove (Visual obj) // removes visual media from dataset
    {
        media.remove ((Media) obj); // removes media from list
        notify_observers (); // notifies observers
    }
    public void remove (Non_visual obj) // removes non-visual media from dataset
    {
        media.remove ((Media) obj); // removes media from list
        notify_observers (); // notifies observers
    }
    public void register (Observer o)  // registers observer
    {
        observers.add (o);  // adds observer to list
    }
    public void remove_observer (Observer o) // removes observer
    {
        observers.remove (o); // removes observer from list
        o.clear_list (); // clears the observer's list
    }
    public void notify_observers () // notifies observers             
    {
        for (Observer observer : observers) 
        {
            observer.update (media); // updates observer
        }
    }
}
