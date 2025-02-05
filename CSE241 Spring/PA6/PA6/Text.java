public class Text extends Media implements Non_playable, Non_visual // text class
{
    public Text (String name, String other_info) // constructor
    {
        super (name, other_info); // calls superclass constructor
    }
    @Override
    public void info () // displays information
    {
        System.out.println (get_name () + " " + get_other_info ());
    }
    @Override
    public void view () // views media
    {
        System.out.println ("Viewing text: " + get_name ());
    }
    @Override
    public void show () // shows media
    {
       view ();
    }
}
