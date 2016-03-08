#include "classes/environment_configuration.hpp"

using namespace tesis;
using namespace boost;

EnvironmentConfiguration::EnvironmentConfiguration( std::string path ) : path( path )
{
    this->parse();
}

EnvironmentConfig EnvironmentConfiguration::get()
{
    return this->configuration;
}

void EnvironmentConfiguration::parse()
{
    // http://theboostcpplibraries.com/boost.propertytree
    try
    {
        EnvironmentConfig config;

        property_tree::ptree ptree;
        property_tree::read_json( this->path, ptree );

        Size space_size;

	//busco varios colores para el robot.
	// We are not considering unsafe spots yet.
        BOOST_FOREACH( property_tree::ptree::value_type & val, ptree.get_child( "robot_id" ) )
        {
            property_tree::ptree child = val.second;
	    
	    
	    Color robot_id;
	    Range r_hue;
	    Range r_saturation;
	    Range r_value;

	    r_hue.min = child.get( "Hue.min", 0 );
	    r_hue.max = child.get( "Hue.max", 0 );

	    r_saturation.min = child.get( "Saturation.min", 0 );
	    r_saturation.max = child.get( "Saturation.max", 0 );

	    r_value.min = child.get( "Value.min", 0 );
	    r_value.max = child.get( "Value.max", 0 );

	    robot_id.Hue = r_hue;
	    robot_id.Saturation = r_saturation;
	    robot_id.Value = r_value;
	    
	    config.robot_id.push_back( robot_id );
	}
	
        space_size.height = ptree.get<float>("space.height");
        space_size.width = ptree.get<float>("space.width");
        
        config.camera_height = ptree.get<float>( "camera_height" );
        config.camera_number = ptree.get( "camera_number", 0 );
	config.path_videos = ptree.get( "path_videos", "" );
	 //     config.robot_id = robot_id;
        config.space = space_size;
        
        // We are not considering unsafe spots yet.
        BOOST_FOREACH( property_tree::ptree::value_type & val, ptree.get_child( "safe_spot" ) )
        {
            property_tree::ptree child = val.second;

            Spot s_spot;
            Range s_hue;
            Range s_saturation;
            Range s_value;
	    
	    s_spot.altitude = child.get( "Altitude", 0);
	    
            s_hue.min = child.get( "Hue.min", 0 );
            s_hue.max = child.get( "Hue.max", 0 );

            s_saturation.min = child.get( "Saturation.min", 0 );
            s_saturation.max = child.get( "Saturation.max", 0 );

            s_value.min = child.get( "Value.min", 0 );
            s_value.max = child.get( "Value.max", 0 );

            s_spot.color.Hue = s_hue;
            s_spot.color.Saturation = s_saturation;
            s_spot.color.Value = s_value;
  
            config.safe_spot.push_back( s_spot );
        }

        this->configuration = config;
    }
    catch( std::exception const& e )
    {
        std::cerr << e.what() << std::endl;
    }
}

EnvironmentConfiguration::~EnvironmentConfiguration()
{

}

