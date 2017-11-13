#include <sstream>
#include <iostream>
#include "NLTemplate.h"



static void testFile() {
    const char *titles[ 3 ] = { "Chico", "Harpo", "Groucho" };
    const char *details[ 3 ] = { "Red", "Green", "Blue" };
    
    NL::Template::LoaderFile loader; // Let's use the default loader that loads files from disk.
    
    NL::Template::Template t( loader );
    
    t.load( "Templates/test.txt" );               // Load & parse the main template and its dependencies.
    t.set( "text", "Hello, world" );    // Set a top-level variable
    t.block( "items" ).repeat( 3 );     // We need to know in advance that the "items" block will repeat 3 times.
    
    // Let's fill in the data for the repeated block.
    for ( int i=0; i < 3; i++ ) {
        // Set title and text by accessing the variable directly
        t.block( "items" )[ i ].set( "title", titles[ i ] );
        t.block( "items" )[ i ].set( "text", "Lorem Ipsum" );
        
        // We can get a shortcut reference to a nested block
        NL::Template::Block & block = t.block( "items" )[ i ].block( "detailblock" );
        block.set( "detail", details[ i ] );
        
        // Disable this block for the first item in the list. Can be useful for opening/closing HTML tables etc.
        if ( i==0 ) {
            block.disable();
        }
    }
    
    std::stringbuf buf;
    std::ostream sout( &buf );
    t.render( sout ); // Render the template with the variables we've set above
    
    std::cout << buf.str();
}


static void testMemory() {
    NL::Template::LoaderMemory loader;
    
    NL::Template::Template t( loader );
    
    loader.add( "base", "<html><head>{{ title }}</head>\n<body><p>{% include text %}</p></body></html>\n" );
    loader.add( "text", "Hi there, {{ name }}. How are you??" );
    
    t.load( "base" );
    t.set( "title", "Testing memory loader" );
    t.set( "name", "Stranger" );
    
    t.render( std::cout );
}


int main(int, char *[] ) {
    testFile();
    testMemory();
   
    
    return 0;
}
