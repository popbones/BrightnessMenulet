//
//  Screen.m
//  BrightnessMenulet
//
//  Created by Kalvin Loc on 1/30/16.
//
//

#import "Screen.h"

@interface Screen ()

@property (strong, readwrite) NSString* model;
@property (readwrite) CGDirectDisplayID screenNumber;
@property (strong, readwrite) NSString* serial;

@property (readwrite) NSInteger currentBrightness;
@property (readwrite) NSInteger maxBrightness;

@property (readwrite) NSInteger currentContrast;
@property (readwrite) NSInteger maxContrast;

@property BOOL* stop, running;

@property (strong, readwrite) NSString* currentAutoAttribute;


@end

@implementation Screen

- (instancetype)initWithModel:(NSString*)model screenID:(CGDirectDisplayID)screenID serial:(NSString*)serial {
    if((self = [super init])){
        _model = [model copy];
        self.screenNumber = screenID;
        _serial = [serial copy];
        
        _running = NO;
        _stop = NO;

        _brightnessOutlets = [NSMutableArray array];
        _contrastOutlets = [NSMutableArray array];
        
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
        if([[[defaults dictionaryRepresentation] allKeys] containsObject:[NSString stringWithFormat: @"autoAttribute_%@", self.model]]){
            _currentAutoAttribute = [defaults stringForKey:[NSString stringWithFormat: @"autoAttribute_%@", self.model]];
        } else {
            [defaults setObject:@"BR" forKey:[NSString stringWithFormat: @"autoAttribute_%@", self.model]];
            _currentAutoAttribute = @"BR";
        }
    }

    return self;
}

- (void)refreshValues {
    struct DDCReadResponse cBrightness = [controls readDisplay:self.screenNumber controlValue:BRIGHTNESS];
    struct DDCReadResponse cContrast   = [controls readDisplay:self.screenNumber controlValue:CONTRAST];

    if (!cBrightness.valid && !cContrast.valid)
        return;

    self.currentBrightness = cBrightness.current_value;
    self.maxBrightness = cBrightness.max_value;

    self.currentContrast = cContrast.current_value;
    self.maxContrast = cContrast.max_value;

    [self updateBrightnessOutlets:_brightnessOutlets];
    [self updateContrastOutlets:_contrastOutlets];

    NSLog(@"Screen: %@ outlets set to BR %ld / CON %ld", _model , (long)self.currentBrightness, (long)self.currentContrast);
}

- (void)ddcReadOut {
    for(int i=0x00; i<=255; i++){
        struct DDCReadResponse response = [controls readDisplay:self.screenNumber controlValue:i];
        NSLog(@"VCP: %x - %d / %d \n", i, response.current_value, response.max_value);
    }
}

- (void)setBrightness:(NSInteger)brightness {
    if(brightness > self.maxBrightness)
        brightness = self.maxBrightness;

    [controls changeDisplay:self.screenNumber control:BRIGHTNESS withValue: brightness];
    self.currentBrightness = brightness;

    NSLog(@"Screen: %@ - %ud Brightness changed to %ld", _model, self.screenNumber, (long)self.currentBrightness);
}

- (void)setBrightnessWithPercentage:(NSInteger)percentage byOutlet:(NSView*)outlet {
    [self setBrightness:((self.maxBrightness) * ((double)(percentage)/100)) byOutlet:outlet];
}

- (void)setBrightness:(NSInteger)brightness byOutlet:(NSView*)outlet {
    if(brightness == self.currentBrightness)
        return;
    else
        [self setBrightness:brightness];

    NSMutableArray* dirtyOutlets = [_brightnessOutlets mutableCopy];
    if(outlet)
        [dirtyOutlets removeObject:outlet];

    [self updateBrightnessOutlets:dirtyOutlets];
}

- (void)updateBrightnessOutlets:(NSArray*)outlets {
    dispatch_async(dispatch_get_main_queue(), ^{
        for(id outlet in outlets){
            if(![outlet isKindOfClass:[NSTextField class]])
                [outlet setMaxValue:_maxBrightness];
            
            [outlet setIntegerValue:_currentBrightness];
        }
    });
}

- (void)setContrast:(NSInteger)contrast {
    if(contrast > self.maxContrast)
        contrast = self.maxContrast;
    
    while (_running) {
        _stop = YES;
    }
    _stop = NO;
    
    int startContrast = self.currentContrast;
    float transitionTime = 0.25; //ms
    int diff = contrast - startContrast;
    float time = transitionTime / abs(diff);
    
    dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        _running = YES;
        for (int i = startContrast; (diff>0) ? i<=contrast : i>=contrast; (diff>0) ? i++ : i--) {
            if (_stop) {
                _running = NO;
                return;
            }
            [controls changeDisplay:self.screenNumber control:CONTRAST withValue: i];
            self.currentContrast = contrast;
            NSLog(@"contrastStep: %d", i);
            [NSThread sleepForTimeInterval:time];
            //sleep(1);
        }
        _running = NO;
        NSLog(@"Screen: %@ - %ud Contrast changed to %ld", _model, self.screenNumber, (long)self.currentContrast);
    });
    
    
    //[controls changeDisplay:self.screenNumber control:CONTRAST withValue: contrast];
    //self.currentContrast = contrast;

}

- (void)setContrastWithPercentage:(NSInteger)percentage byOutlet:(NSView*)outlet {
    [self setContrast:(self.maxContrast * ((double)(percentage)/100)) byOutlet:outlet];
}

- (void)setContrast:(NSInteger)contrast byOutlet:(NSView*)outlet {
    if(contrast == self.currentContrast)
        return;
    else
        [self setContrast:contrast];

    NSMutableArray* dirtyOutlets = [_contrastOutlets mutableCopy];
    if(outlet)
        [dirtyOutlets removeObject:outlet];

    [self updateContrastOutlets:dirtyOutlets];
}

- (void)updateContrastOutlets:(NSArray*)outlets {
    dispatch_async(dispatch_get_main_queue(), ^{
        for(id outlet in outlets){
            if(![outlet isKindOfClass:[NSTextField class]])
                [outlet setMaxValue:_maxContrast];
            
            [outlet setIntegerValue:_currentContrast];
        }
    });
}

- (void)setAutoAttribute:(NSString*)attribute {
    self.currentAutoAttribute = attribute;
    
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSLog(@"setAutoAttribute: %@ ", self.currentAutoAttribute);
    [defaults setObject:self.currentAutoAttribute forKey:[NSString stringWithFormat: @"autoAttribute_%@", self.model]];
    
}


@end
