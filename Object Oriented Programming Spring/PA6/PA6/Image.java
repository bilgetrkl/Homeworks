public class Image extends Media implements Non_playable, Visual // image class
{
    private String dimension_info; // dimension information

    public Image (String name, String dimension_info, String other_info) // constructor
    {
        super (name, other_info); // calls superclass constructor
        this.dimension_info = dimension_info;
    }
    @Override
    public void info () // displays information
    {
        System.out.println (get_name () + " " + dimension_info + " " + get_other_info ());
    }
    @Override
    public void display () // displays image
    {
        System.out.println ("Displaying image: " + get_name ());
    }
    @Override
    public void show () // displays image
    {
        display ();
    }
}
