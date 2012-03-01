#import "RevBayes.h"
#import "ScalingScrollView.h"
#import "ToolView.h"




@implementation ToolView

@synthesize whichToolSelected;

- (void)awakeFromNib {

    for (int i=0; i<numTools; i++)
        {
		NSRect toolRect;
		toolRect.origin = NSMakePoint(i * toolSize, 2.0);
		toolRect.size   = NSMakeSize(toolSize, toolSize);
		NSTrackingArea* ta = [[NSTrackingArea alloc] initWithRect:toolRect 
													      options:(NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp)
													    	owner:self
													     userInfo:nil];
		[self addTrackingArea:ta];
		[myTrackingAreas addObject:ta];
		}
}

-(void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
	NSEnumerator* enumerator = [myTrackingAreas objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        [self removeTrackingArea:element];
	[myTrackingAreas release];
	[super dealloc];
}

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)isLocal {

	return NSDragOperationCopy;
}

- (void)drawRect:(NSRect)dirtyRect {

    // draw the background
	NSRect bounds = [self bounds];
	NSColorList* ColorListToUse = [NSColorList colorListNamed:@"Crayons"];
	[[ColorListToUse colorWithKey:@"Silver"] set];
	[NSBezierPath fillRect:bounds];

    // draw the tools
    for (int i=0; i<numTools; i++)
        {
        // get a pointer to the tool image
        NSImage* toolImage;
        if (i == TOOL_LOOP)
            toolImage = [NSImage imageNamed:@"Tool_Loop.icns"];
        else if (i == TOOL_READDATA)
            toolImage = [NSImage imageNamed:@"Tool_ReadData.icns"];
        else if (i == TOOL_ALIGN)
            toolImage = [NSImage imageNamed:@"Tool_Align.icns"];
        else if (i == TOOL_COMBINER)
            toolImage = [NSImage imageNamed:@"Tool_Combiner.icns"];
        else if (i == TOOL_CONCATENATE)
            toolImage = [NSImage imageNamed:@"Tool_Combiner.icns"];
        else if (i == TOOL_MATRIXFILTER)
            toolImage = [NSImage imageNamed:@"Tool_MatrixFilter.icns"];
        else if (i == TOOL_BOOTSTRAP)
            toolImage = [NSImage imageNamed:@"Tool_Bootstrap.icns"];
        else if (i == TOOL_MODEL)
            toolImage = [NSImage imageNamed:@"Tool_Model.icns"];
        else if (i == TOOL_MCMC)
            toolImage = [NSImage imageNamed:@"Tool_Mcmc.icns"];
        else if (i == TOOL_SIMULATE)
            toolImage = [NSImage imageNamed:@"Tool_Simulate.icns"];
        else if (i == TOOL_DISTANCEMATRIX)
            toolImage = [NSImage imageNamed:@"Tool_Distance.icns"];
        else if (i == TOOL_NEIGHBORJOINING)
            toolImage = [NSImage imageNamed:@"Tool_NeighborJoining.icns"];
        else if (i == TOOL_PARSIMONY)
            toolImage = [NSImage imageNamed:@"Tool_Parsimony.icns"];
        else if (i == TOOL_TREECONTAINER)
            toolImage = [NSImage imageNamed:@"Tool_TreeSummarize.icns"];
        else if (i == TOOL_NUMBERSET)
            toolImage = [NSImage imageNamed:@"ToolNumberSet.icns"];
        else if (i == TOOL_TREEDIAGNOSIS)
            toolImage = [NSImage imageNamed:@"Tool_TreeDiagnosis.icns"];
        else if (i == TOOL_NUMBERDIAGNOSIS)
            toolImage = [NSImage imageNamed:@"Tool_NumberSetDiagnosis.icns"];
            
        [toolImage setSize:NSMakeSize(toolSize, toolSize)];

        // get the rectangle for drawing
		NSRect imageRect;
		imageRect.origin = NSZeroPoint;
		imageRect.size = NSMakeSize(toolSize, toolSize);
		NSRect drawingRect = imageRect;
		drawingRect.origin = NSMakePoint(i * toolSize, 13.0);
        
		// draw the tool image
		[toolImage drawInRect:drawingRect fromRect:imageRect operation:NSCompositeSourceOver fraction:1.0];
        }
		
	if (isToolTipTimerActive == YES && toolForToolTip >= 0)
		{
		NSPoint p;
		p.x = toolForToolTip * toolSize + toolSize * 0.5;
		p.y = toolSize * 0.75;
	
		NSString* myTip;
        if (toolForToolTip == TOOL_LOOP)
			myTip = [NSString stringWithString:@" Loop Tool "];
        else if (toolForToolTip == TOOL_READDATA)
			myTip = [NSString stringWithString:@" Read Data Tool "];
        else if (toolForToolTip == TOOL_ALIGN)
			myTip = [NSString stringWithString:@" Sequence Alignment Tool "];
        else if (toolForToolTip == TOOL_COMBINER)
			myTip = [NSString stringWithString:@" Matrix Combiner Tool "];
        else if (toolForToolTip == TOOL_CONCATENATE)
			myTip = [NSString stringWithString:@" Alignment Concatenation Tool "];
        else if (toolForToolTip == TOOL_MATRIXFILTER)
			myTip = [NSString stringWithString:@" Data Matrix Filter Tool "];
        else if (toolForToolTip == TOOL_BOOTSTRAP)
			myTip = [NSString stringWithString:@" Bootstrap Tool "];
        else if (toolForToolTip == TOOL_MODEL)
			myTip = [NSString stringWithString:@" Model Tool "];
        else if (toolForToolTip == TOOL_MCMC)
			myTip = [NSString stringWithString:@" MCMC Tool "];
        else if (toolForToolTip == TOOL_SIMULATE)
			myTip = [NSString stringWithString:@" Simulate Data Tool "];
        else if (toolForToolTip == TOOL_DISTANCEMATRIX)
			myTip = [NSString stringWithString:@" Distance Matrix Tool "];
        else if (toolForToolTip == TOOL_NEIGHBORJOINING)
			myTip = [NSString stringWithString:@" Neighbor Joining Tool "];
        else if (toolForToolTip == TOOL_PARSIMONY)
			myTip = [NSString stringWithString:@" Parsimony Tool "];
        else if (toolForToolTip == TOOL_TREECONTAINER)
			myTip = [NSString stringWithString:@" Tree Container Tool "];
        else if (toolForToolTip == TOOL_NUMBERSET)
			myTip = [NSString stringWithString:@" Number Container Tool "];
        else if (toolForToolTip == TOOL_TREEDIAGNOSIS)
			myTip = [NSString stringWithString:@" Tree Diagnosis Tool "];
        else if (toolForToolTip == TOOL_NUMBERDIAGNOSIS)
			myTip = [NSString stringWithString:@" Number Container Diagnosis Tool "];
            
		NSDictionary *attr = [NSDictionary 
					 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                              forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
		NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];
		NSRect tipRect = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:(NSStringDrawingUsesLineFragmentOrigin)];
		tipRect.origin = p;
        NSRect vRect = [self visibleRect];
        if (tipRect.origin.x + tipRect.size.width > vRect.origin.x + vRect.size.width)
            {
            tipRect.origin.x = vRect.origin.x + vRect.size.width - (tipRect.size.width + 10.0);
            p = tipRect.origin;
            }
				
		[[[NSColor darkGrayColor] colorWithAlphaComponent:0.75] set];
		[NSBezierPath fillRect:tipRect];
		[attrString drawAtPoint:p];
		[attrString release];
		}
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{
        // register this view as drag source for an image
        [self registerForDraggedTypes:[NSImage imagePasteboardTypes]];

		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(scaleFactorChanged:)
							  name:@"scaleFactorChangeNotification"
						    object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSWindowDidResizeNotification"
							object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSSplitViewDidResizeSubviewsNotification"
							object:nil];
							
		// allocate the tool tracking rects
		myTrackingAreas = [[NSMutableArray alloc] init];

		// set default values for object variables
        numTools             = NUM_TOOLS;
        toolSize             = 100.0;
		whichToolSelected    = -1;
        scaleFactor          = 1.0;
		toolTipTimer         = nil;
		isToolTipTimerActive = NO;
		}
    return self;
}

- (void)mouseDown:(NSEvent *)event {

	[self stopToolTipTimer];
	
    // get the mouse down point
    NSPoint p = [event locationInWindow];
    p = [self convertPoint:p fromView:nil];
    
    // which tool is selected
    whichToolSelected = p.x / toolSize;
}

- (void)mouseDragged:(NSEvent*)event {
	    
    NSPoint down = [mouseDownEvent locationInWindow];
    NSPoint drag = [event locationInWindow];
    float distance = hypot(down.x-drag.x, down.y-drag.y);
    if ( distance < 3.0 )
        return;
    
    // create the image that will be dragged
    NSSize s = NSMakeSize(ITEM_IMAGE_SIZE*scaleFactor, ITEM_IMAGE_SIZE*scaleFactor);
	NSImage* anImage;
	if (whichToolSelected == TOOL_LOOP)
		anImage = [NSImage imageNamed:@"Tool_Loop.icns"];
	else if (whichToolSelected == TOOL_READDATA)
		anImage = [NSImage imageNamed:@"Tool_ReadData.icns"];
	else if (whichToolSelected == TOOL_ALIGN)
		anImage = [NSImage imageNamed:@"Tool_Align.icns"];
	else if (whichToolSelected == TOOL_COMBINER)
		anImage = [NSImage imageNamed:@"Tool_Combiner.icns"];
	else if (whichToolSelected == TOOL_CONCATENATE)
		anImage = [NSImage imageNamed:@"Tool_Combiner.icns"];
	else if (whichToolSelected == TOOL_MATRIXFILTER)
		anImage = [NSImage imageNamed:@"Tool_MatrixFilter.icns"];
	else if (whichToolSelected == TOOL_BOOTSTRAP)
		anImage = [NSImage imageNamed:@"Tool_Bootstrap.icns"];
	else if (whichToolSelected == TOOL_MODEL)
		anImage = [NSImage imageNamed:@"Tool_Model.icns"];
	else if (whichToolSelected == TOOL_MCMC)
		anImage = [NSImage imageNamed:@"Tool_Mcmc.icns"];
	else if (whichToolSelected == TOOL_SIMULATE)
		anImage = [NSImage imageNamed:@"Tool_Simulate.icns"];
	else if (whichToolSelected == TOOL_DISTANCEMATRIX)
		anImage = [NSImage imageNamed:@"Tool_Distance.icns"];
	else if (whichToolSelected == TOOL_NEIGHBORJOINING)
		anImage = [NSImage imageNamed:@"Tool_NeighborJoining.icns"];
	else if (whichToolSelected == TOOL_PARSIMONY)
		anImage = [NSImage imageNamed:@"Tool_Parsimony.icns"];
	else if (whichToolSelected == TOOL_TREECONTAINER)
		anImage = [NSImage imageNamed:@"Tool_TreeSummarize.icns"];
	else if (whichToolSelected == TOOL_NUMBERSET)
		anImage = [NSImage imageNamed:@"ToolNumberSet.icns"];
	else if (whichToolSelected == TOOL_TREEDIAGNOSIS)
		anImage = [NSImage imageNamed:@"Tool_TreeDiagnosis.icns"];
	else if (whichToolSelected == TOOL_NUMBERDIAGNOSIS)
		anImage = [NSImage imageNamed:@"Tool_NumberSetDiagnosis.icns"];
	else 
		NSLog(@"Problem deciding which tool is to be dragged");

    [anImage setSize:s];
    
    // get the location of the mouseDown event
    NSPoint p = [self convertPoint:down fromView:nil];
    
    // drag from the center of the image
    p.x -= s.width/2.0;
    p.y -= s.height/2.0;

    // get the pasteboard
    NSPasteboard* pb = [NSPasteboard pasteboardWithName:NSDragPboard];
    
    // put the image on the pasteboard
    [pb declareTypes:[NSArray arrayWithObject: NSTIFFPboardType] owner:self];
    [pb addTypes:[NSArray arrayWithObject:NSPDFPboardType] owner:self];

    // start the drag
	[self dragImage:anImage 
	             at:p 
			 offset:NSMakeSize(0.0, 0.0) 
			  event:mouseDownEvent 
		 pasteboard:pb 
		     source:self 
		  slideBack:NO];
}

- (void)mouseEntered:(NSEvent*)event {

    NSPoint p = [event locationInWindow];
    p = [self convertPoint:p fromView:nil];
	potentialToolForToolTip = p.x / toolSize;
	[self startToolTipTimer];
}

- (void)mouseExited:(NSEvent*)event {
	
	[self stopToolTipTimer];
}

- (void)scaleFactorChanged:(NSNotification*)notification {

    scaleFactor = [[notification object] scaleFactor];
}

- (void)startToolTipTimer {

    //[analysisDocumentPtr invalidateTimer];
    [self stopToolTipTimer];
    
    toolTipTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(toolTipFire) userInfo:nil repeats:YES];
    isToolTipTimerActive = YES;
    fireNum = 0;
}

- (void)stopToolTipTimer {
 
    if (isToolTipTimerActive == YES)
        {
        [toolTipTimer invalidate];
        isToolTipTimerActive = NO;
        toolTipTimer = nil;
        toolForToolTip = -1;
        }
	[self setNeedsDisplay:YES];
}

- (void)toolTipFire {

    fireNum++;
    if (fireNum == 1)
        {
		toolForToolTip = potentialToolForToolTip;
        [self setNeedsDisplay:YES];
        }
    else if (fireNum == 6)
        {
        [self stopToolTipTimer];
        }
}

- (void)updateScrollBars {

    // get information on the bounds and used area
	NSRect visible = [self visibleRect];
    float toolX = toolSize * numTools;

    // update the size of the frame
	NSRect newFrame = visible;
	if ( visible.size.width < toolX )
		newFrame.size.width = toolX;
	[self setFrame:newFrame];
}

- (void)windowDidResize:(NSNotification *)notification {

    [self updateScrollBars];
}

@end
