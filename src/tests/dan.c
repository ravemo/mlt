
#include <framework/mlt.h>

#include <stdio.h>

int main( int argc, char **argv )
{
	char temp[ 132 ];
	char *file1 = NULL;
	char *file2 = NULL;

	mlt_factory_init( "../modules" );

	if ( argc >= 2 )
		file1 = argv[ 1 ];
	if ( argc >= 3 )
		file2 = argv[ 2 ];

	// Start the consumer...
	int vstd = mlt_video_standard_ntsc;
	mlt_consumer consumer = mlt_factory_consumer( "bluefish", &vstd );
	mlt_properties_set_int( mlt_consumer_properties( consumer ), "video_standard", mlt_video_standard_ntsc );

	// Create the producer(s)
	mlt_producer dv1 = mlt_factory_producer( "mcmpeg", file1 );

#if 0
	// Connect the tractor to the consumer
	mlt_consumer_connect( consumer, mlt_producer_service( dv1 ) );

	// Do stuff until we're told otherwise...
	fprintf( stderr, "Press return to continue\n" );
	fgets( temp, 132, stdin );
	mlt_consumer_close( consumer );
	return 0;
#endif

	//mlt_producer dv1 = producer_pixbuf_init( file1 );
	//mlt_producer dv2 = producer_libdv_init( file2 );
	//mlt_producer dv2 = mlt_factory_producer( "pixbuf", file2 );
	mlt_producer dv2 = mlt_factory_producer( "pango", "<span font_desc=\"Sans Bold 36\">Mutton <span font_desc=\"Luxi Serif Bold Oblique 36\">Lettuce</span> Tomato</span>" );
	mlt_properties_set_int( mlt_producer_properties( dv2 ), "video_standard", mlt_video_standard_ntsc );
	mlt_properties_set_int( mlt_producer_properties( dv2 ), "bgcolor", 0x0000007f );
	mlt_properties_set_int( mlt_producer_properties( dv2 ), "pad", 8 );

	// Register producers(s) with a multitrack object
	mlt_multitrack multitrack = mlt_multitrack_init( );
	mlt_multitrack_connect( multitrack, dv1, 0 );
	mlt_multitrack_connect( multitrack, dv2, 1 );

	// Create a filter and associate it to track 0
//	mlt_filter filter = mlt_factory_filter( "deinterlace", NULL );
//	mlt_filter_connect( filter, mlt_multitrack_service( multitrack ), 0 );
//	mlt_filter_set_in_and_out( filter, 0, 1000 );

	// Define a transition
	mlt_transition transition = mlt_factory_transition( "composite", NULL );
	mlt_transition_connect( transition, mlt_multitrack_service( multitrack ), 0, 1 );
	mlt_transition_set_in_and_out( transition, 0, 1000 );

	// Buy a tractor and connect it to the filter
	mlt_tractor tractor = mlt_tractor_init( );
	mlt_tractor_connect( tractor, mlt_transition_service( transition ) );

	// Connect the tractor to the consumer
	mlt_consumer_connect( consumer, mlt_tractor_service( tractor ) );

	// Do stuff until we're told otherwise...
	fprintf( stderr, "Press return to continue\n" );
	fgets( temp, 132, stdin );

	// Close everything...
	mlt_consumer_close( consumer );
	//mlt_tractor_close( tractor );
	//mlt_filter_close( filter );
	//mlt_multitrack_close( multitrack );
	//mlt_producer_close( dv1 );
	//mlt_producer_close( dv2 );

	return 0;
}
