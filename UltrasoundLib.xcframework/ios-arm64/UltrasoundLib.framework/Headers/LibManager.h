#import <UIKit/UIKit.h>

/**
 * Define a LibManagerDelegate protocol
 *
 */
@protocol LibManagerDelegate
@required

/**
 * Callback ultrasound image provided by Kernel
 *
 *@param buff byte array data
 *@param buff2 reserved
 *@param width ultrasound image width
 *@param height ultrasound image height
 *
 */
-(void)libManagerWithRawImage:(unsigned char*)buff raw:(unsigned char*)buff2 Width:(int)width Height:(int)height;

/**
 * Set System Listener for Initialization
 *
 * @param eventID event ID
 */
-(void) SystemListenerWithEventID:(int)eventID errorCode:(int) errorCode value:(float) val;

/**
 * Set ScanListener
 *
 * @param eventID event ID
 */
-(void) ScanListenerWithEventID:(int)eventID value:(float) val;

/**
 * Set InfoListener to obtain the Probe's own temperature, battery power, button presses, etc.
 *
 * @param eventID event ID
 */
-(void) InfoListenerWithEventID:(int)eventID value:(float) val;

/**
 * Callback ultrasound message provided by Kernel
 *
 *@param buff byte array data
 *
 */
-(void)libManagerMessage:(const char*)buff;

@end

@interface LibManager : NSObject
+(instancetype)shared;
//MARK: - Control by controller itself -
- (instancetype)initWithDelegate:(id)_delegate;
-(void)set_Delegate:(id)_delegate;
-(void)removeDelegate;

/**
 * Initial kernel library
 *
 */
- (void)initLib;

/**
 * FEBE.db Database update
 *
 */
-(void)isFEBEExistsAtPath;

/**
 * Connect the probe
 *
 */
-(void)connect: (int) force_wifi_channel WiFi5G: (int)force_wifi_channel5G ssid:(NSString*)_ssid enable5GHzSwitchPrompt:(bool)_enable5GHzSwitchPrompt;

/**
 * Set the address of the probe for Wi-Fi connection(reserved)
 *
 * @param _addr address
 */
-(void)set_Address:(NSString*)_addr;

/**
 * Set the control port No. 1 of the probe for Wi-Fi connection(reserved)
 *
 * @param _port1 port number
 */
-(void)set_Port1:(NSString*)_port1;

/**
 * Set the control port No. 2 of the probe for Wi-Fi connection(reserved)
 *
 * @param _port2 port number
 */
-(void)set_Port2:(NSString*)_port2;

/**
 * Request to start scan
 * The event ID: ON_SCAN_STARTED returned by ScanListener indicates that the scan was started successfully.
 *
 */
-(void) libStartScan;

/**
 * Request to stop scan
 * The event ID: ON_SCAN_STOPPED returned by ScanListener indicates that the stop scan was successful.
 *
 */
-(void) libStopScan;

/**
 * Reserved function.
 *
 */
-(float) libGetGetPropertiesFloat: (int) id;
/**
 * Get the frame rate of the current image received by the tablet from the Probe, in Hz
 *
 * @return frame rate
 */
-(float) libGetFrameRate;

/**
 *
 * @return 0~100 (%), null means that the value of device has not been obtained yet
 */
-(int) libGetBatteryLevel;

/**
 *
 * @return 0~100 (°C), null means that the value of device has not been obtained yet
 */
-(int) libGetTemperature;

/**
 *
 * @return 0~100 (°C), null means that the value of device has not been obtained yet
 */
-(int) libCheckCurrentTemperature;

/**
 * Get motion sensor data
 *
 * @param  index  the index of the scan lines
 * @return motion sensor data
 */
-(NSString *) libGetMotionSensorData:(int) index;

/**
 * Trying to switch scan mode to B mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_B, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 */
-(void) libSwitchToBMode;

/**
 * Trying to switch scan mode to C mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_C, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 */
-(void) libSwitchToCMode;

/**
 * Trying to switch scan mode to M mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_M, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 */
-(void) libSwitchToMMode;

/**
 * Trying to switch scan mode to PW mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_PW, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 *
 * @param duplex Whether to enable multi-tasking mode
 */
-(void) libSwitchToPWMode: (bool)duplex;

/**
 * Trying to switch scan mode to PD mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_PD, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 */
-(void) libSwitchToPDMode;

/**
 * Trying to switch scan mode to B mode
 * ScanListener returns ON_MODE_SWITCHED, and the read mode is equal to MODE_B, which means the switch is successful.
 * ON_MODE_SWITCHING_ERROR returned by ScanListener indicates switching failure.
 */
-(void) libSwitchToDefaultMode;

/**
 * Return the current scan mode
 *
 * @return MODE_B: 0, MODE_M: 1, MODE_C: 2, MODE_PW: 3, MODE_PD: 4;
 */
-(int) libGetMode;

/**
 * Get the freq currently set in Probe, including B, B mode harmonica, C and PW mode. The unit is MHz.
 * The corresponding frequency will be returned according to the currently set operating mode (B, C or PW mode)
 * @return Freq
 */
-(float) libGetFreq;

/**
 * Requirement setting freq
 * The ScanListener event ID returns ON_FREQ_SET to indicate successful setting.
 * ON_FREQ_SET_ERROR returned by ScanListener event ID indicates setting failure.
 *
 * @param mode Probe's current operating mode, reasonable value MODE_B: 0, MODE_M: 1, MODE_C: 2, MODE_PW: 3, MODE_PD: 4;
 * @param newFreq Reasonable values ​​are set based on the Array returned by libGetAllFreq()
 */
-(void) libSetFreq:(int)mode freq:(float) newFreq;

/**
 * Get the current receiving frequency Rx freq, the unit is MHz
 *
 * @return RxFreq
 */
-(float) libGetRxFreq;

/**
 * Get the current depth setting value in cm
 *
 * @return depth
 */
-(float) libGetDepth;

/**
 * Required to set depth
 * The ScanListener event ID returns ON_DEPTH_SET to indicate successful setting.
 * ON_DEPTH_SET_ERROR returned by ScanListener event ID indicates setting failure.
 *
 * @param fNewDepth Reasonable values ​​are determined by the array obtained by libGetAllDepth()
 */
-(void) libSetDepth:(float) fNewDepth;

/**
 * request to set DepthPWmode percentage in depth
 *
 * @param depthPercentage percentage in depth
 */
-(void) libSetDepthPWmode:(float) depthPercentage;

/**
 * request to get decimation in depth
 *
 * @return decimation
 */
-(int) libGetDecimation;

/**
 * return selectable decimations array for PW mode
 *
 * @param p_nAllDecimationPWmode array to get decimation for PW mode
 * @param nLength array number assigned to get decimation data
 *
 * @return Array of number all of the possible decimation data for PW mode
 */
-(int) libGetAllDecimationPWmode:(int *)p_nAllDecimationPWmode nLength:(const int) nLength;

/**
 * return the current used decimation for PW mode
 *
 * @return decimationPWmode
 */
-(int) libGetDecimationPWmode;

/**
 * request to set decimationPWmode
 *
 * @param nDecimation decimation data
 */
-(void) libSetDecimationPWmode:(int) nDecimation;

/**
 * Get PW mode image
 *
 * @param idx index to show the idx's frame of PW mode image
 * @param isDraw true: enable auto envelope; false: disable auto envelope
 * @return PW mode image data
 */
-(unsigned char*) libGetPWEnvImage:(int) idx isDraw:(bool) isDraw;

/**
 * Return real time PW mode measurement data array
 *
 * @param p_nAutoMeasure data as below:
 *     p_nAutoMeasure[0] = mv.f_SD;
 *     p_nAutoMeasure[1] = mv.f_EDV;
 *     p_nAutoMeasure[2] = mv.f_PSV;
 *     p_nAutoMeasure[3] = mv.f_TAV;
 *     p_nAutoMeasure[4] = mv.f_PI;
 *     p_nAutoMeasure[5] = mv.f_RI;
 *     p_nAutoMeasure[6] = mv.f_VTI;
 *     p_nAutoMeasure[7] = mv.f_SB;
 *     p_nAutoMeasure[8] = mv.i_HR;
 *     p_nAutoMeasure[9] = mv.f_ACCL;
 *     p_nAutoMeasure[10] = mv.f_ACCT;
 *     p_nAutoMeasure[11] = mv.f_VFM;
 *     p_nAutoMeasure[12] = mv.f_VFC;
 *     p_nAutoMeasure[13] = mv.f_VFCMax;
 *     p_nAutoMeasure[14] = mv.f_VFD;
 *
 * @param nLength array length to get PW mode measurement data
 *
 * @return real time PW mode measurement data array length
 */
-(float) libGetAutoMeasure:(float *)p_nAutoMeasure nLength:(const int) nLength;

/**
 * Turn on/off PW auto Envelope
 *
 * @param  isDraw true: open auto-envelope function; false: close auto-envelope function
 * @param isCarotid needs to be set true
 */
-(void) libOpenPWRealTimeEnvelopeLine:(bool) isDraw isCarotid:(bool) isCarotid;

/**
 * Set PW drawing property (always by default setting)
 *
 */
-(void) libSetPWRealTimeDrawLineDetails;

/**
 * Get whether the kernel has enabled auto envelope
 *
 *  @return true: enabled; false: not enabled
 */
-(bool) libGetDrawWrapper;

/**
 * return all gate array that can be set for PW mode
 *
 * @param p_nAllGatePWmode array to get gate for PW mode
 * @param nLength array number assigned to get gate data
 *
 * @return Array of number all of the possible gate data for PW mode
 */
-(int) libGetAllGatePWmode:(int *)p_nAllGatePWmode nLength:(const int) nLength;

/**
 * return the current used gate for PW mode
 *
 * @return gatePWmode
 */
-(int) libGetGatePWmode;

/**
 * request to set gatePWmode ( call this before pw mode switched  )
 *
 * @param gate PW gate
 */
-(void) libSetGatePWmode:(int) gate;

/**
 * return selectable ensembleNum array for PW mode
 *
 * @param p_nAllEnsemblePWmode array to get ensembleNum for PW mode
 * @param nLength array number assigned to get ensembleNum data
 */
-(int) libGetAllEnsemblePWmode:(int *)p_nAllEnsemblePWmode nLength:(const int) nLength;

/**
 * return the current used ensembleNum for PW mode
 *
 * @return ensemblePWmode
 */
-(int) libGetEnsemblePWmode;

/**
 * request to set EnsemblePWmode ( call this before  pw mode switched )
 *
 * @param ensembleNum ensembleNum for PW mode
 */
-(void) libSetEnsemblePWmode:(int) ensembleNum;

/**
 * return selectable Baseline array for PW mode
 *
 * @param p_nAllBaselinePWmode array of Baseline for PW mode
 * @param nLength array length to get PW mode baseline
 *
 * @return allBaselinePWmode
 *
 * @warning This function is no longer used
 */
-(int) libGetAllBaselinePWmode:(int *)p_nAllBaselinePWmode nLength:(const int) nLength;

/**
 * return the current used Baseline for PW mode
 *
 * @return BaselinePWmode
 */
-(int) libGetBaselinePWmode;

/**
 * return the auto-used Baseline for PW mode 0~100
 *
 * @return AutoBaselinePWmode
 */
-(int) libGetBaselinePWAuto;

/**
 * return if the PW waveform is reversed
 *
 * @return true for reversed; false for not reversed
 */
-(bool) libGetPWReverse;

/**
 * request to set baseline level for PWmode
 *
 * @param Baseline value: 0~100
 * @param rv > 0 for reverse mode; otherwise for non-reversed mode
 */
-(void) libSetBaselinePWmode:(int) Baseline r: (int) rv;

/**
 * set reverse status in PWmode
 *
 * @param reverse true: reversed; false not reversed
 */
-(void) libSetBaselineReversePWmode:(bool) reverse;

/**
 * return if the PW waveform is reversed
 *
 * @return true for reversed; false for not reversed
 */
-(bool) libGetBaselineReversePWmode;

/**
 * set angle of gate in PWmode
 *
 * @param angle in degree, value: -75 degrees ~ +75 degrees
 */
-(void) libSetGateAnglePWmode:(int) angle;

/**
 * get angle of gate in PWmode
 *
 * @return angle in degree
 */
-(int) libGetGateAnglePWmode;

/**
 * return all persistence that can be set for B mode
 *
 * @param p_nAllPersistenceBmode array to get gate for B mode
 * @param nLength array number assigned to get persistence data
 *
 * @return Array of number all of the possible persistence data for B mode
 */
-(int) libGetAllPersistenceBmode:(int *)p_nAllPersistenceBmode nLength:(const int) nLength;

/**
 * Get the current persistence settings for B mode
 *
 * @return persistence
 */
-(int) libGetPersistenceBmode;

/**
 * Set persistence for B mode
 *
 * @param persistence reasonable value 0 ~ 4
 */
-(void) libSetPersistenceBmode:(int) persistence;

/**
 * Get the currently set image enhancement level
 *
 * @return image enhance level
 */
-(int) libGetEnhanceLevelBmode;

/**
 * Set image enhancement level
 *
 * @param enhanceLevel reasonable value 0 ~ 4, 0: Represents no image processing
 */
-(void) libSetEnhanceLevelBmode:(int) enhanceLevel;

/**
 * Get the currently set PRF setting value, including C mode and PW mode
 *
 * @param mode MODE_C: 2, MODE_PW: 3
 * @return PRF
 */
-(float) libGetColorPrf: (int)mode;

/**
 * Set PRF, including C mode or PW mode
 *
 * @param mode Current operating mode, MODE_C: 2, MODE_PW: 3
 * @param newColorPrf  Reasonable values ​​are determined by libGetAllColorPrf()
 */
-(void) libSetColorPrf:(int)mode prf:(float) newColorPrf;

/**
 * Get the current color Sensitivity setting value
 *
 * @return color Sensitivity
 */
-(int) libGetColorSensitivity;

/**
 * Set color Sensitivity
 *
 * @param newColorSensitivity Reasonable values ​​are determined by libGetAllColorSensitivity()
 */
-(void) libSetColorSensitivity:(int) newColorSensitivity;

/**
 * Get the current CF mode noise (= Color Threshold)
 *
 * @return the noise  of CF mode
 */
-(int) libGetColorNoise;

/**
 *  Get or set the current CF mode Tx aperture
 *
 * opt=0 to use 32 Tx
 * opt=1 to use 48 Txo
 * pt=2 to use 64 Tx
 * opt=-1 to get current state (return by int)
 *
 * @para opt
 * @return the current setting of CF mode Tx aperture
 */
-(int) libGetSetColorTxAperture:(int) opt;

/**
 * Set color Noise (= Color Threshold)
 *
 * @param newColorNoise new color noise
 */
-(void) libSetColorNoise:(int) newColorNoise;

/**
 * Get all the color angles that can be set by the linear probe, the unit is degree
 *
 * @param p_fAllColorAngle array to get color angle
 * @param nLength array number assigned to get color angle data
 */
-(int) libGetAllColorAngle:(float *)p_fAllColorAngle nLength:(const int) nLength;

/**
 * Get the color Angle of the current linear probe
 *
 * @return color Angle
 */
-(float) libGetColorAngle;

/**
 * Set color Angle
 *
 * @param newColorAngle Reasonable values ​​are determined by libGetAllColorAngle()
 */
-(void) libSetColorAngle:(float) newColorAngle;

/**
 * Get the current color angle tangent
 *
 * @return color Angle tan
 */
-(CGFloat) libGetColorAngleTan;

/**
 * Get the current B mode compound number
 *
 * @return the compound number of B mode
 */
-(int) libGetCompoundNumBmode;

/**
 * Set B mode compound number
 *
 * @param num Reasonable values ​​are determined by libGetAllCompoundNumBmode()
 */
-(void) libSetCompoundNumBmode:(int) num;

/**
 * Get the current B mode Tgc Table Count
 *
 * @return B mode tgc table count
 */
-(int) libGetTgcTableBmodeCount;

/**
 * Get the current C mode Tgc Table Count
 *
 * @return C mode tgc table count
 */
-(int) libGetTgcTableCmodeCount;

/**
 * Get all configurable B mode cycle numbers
 *
 * @param p_nAllCycleNumBmode array to get cycle number
 * @param nLength array number assigned to get cycle number data
 */
-(int) libGetAllCycleNumBmode:(float *)p_nAllCycleNumBmode nLength:(const int) nLength;

/**
 * Get the currently set B mode cycle number
 *
 * @return cycleNum
 */
-(float) libGetCycleNumBmode;

/**
 * Required to set B mode cycle number
 *
 * @param num Reasonable value: determined by libGetAllCycleNumBmode() obtained
 */
-(void) libSetCycleNumBmode:(float) num;

/**
 * Get all configurable C mode cycle numbers
 *
 * @param p_nAllCycleNumCmode array to get cycle number
 * @param nLength array number assigned to get cycle number data
 */
-(int) libGetAllCycleNumCmode:(float *)p_nAllCycleNumCmode nLength:(const int) nLength;

/**
 * Get the currently set C mode cycle number
 *
 * @return cycleNum
 */
-(float) libGetCycleNumCmode;

/**
 * Required to set C mode cycle number
 *
 * @param num Reasonable value: determined by libGetAllCycleNumCmode() obtained
 */
-(void) libSetCycleNumCmode:(float) num;

/**
 * Get all configurable PW mode cycle numbers
 *
 * @param p_nAllCycleNumPWmode array to get cycle number
 * @param nLength array number assigned to get cycle number data
 */
-(int) libGetAllCycleNumPWmode:(float *)p_nAllCycleNumPWmode nLength:(const int) nLength;

/**
 * Get the current PW mode cycle number
 *
 * @return cycleNum
 */
-(float) libGetCycleNumPWmode;

/**
 * Required to set PW mode cycle number
 *
 * @param num Reasonable values: determined by libGetAllCycleNumPWmode()
 */
-(void) libSetCycleNumPWmode:(float) num;

/**
 * Get all settable freeze timer arrays, the unit is second
 *
 * @param p_nAllFreezeTimer array to get freezetimer
 * @param nLength array number assigned to get freezetimer data
 */
-(int) libGetAllFreezeTimer:(int *)p_nAllFreezeTimer nLength:(const int) nLength;

/**
 * Get the current freeze timer setting value
 *
 * @return freeze timer
 */
-(int) libGetFreezeTimer;

/**
 * Set freeze timer
 *
 * @param seconds wait to freeze when the probe is idle, reasonable values ​​are determined by libGetAllFreezeTimer()
 */
-(void) libSetFreezeTimer:(int) seconds;

/**
 * Get all frame rates that can be set in B mode, unit is Hz
 *
 * @param p_fAllFrameRateBmode array to get framerate for B mode
 * @param nLength array number assigned to get framerate for B mode data
 */
-(int) libGetAllFrameRateBmode:(float *)p_fAllFrameRateBmode nLength:(const int) nLength;

/**
 * Get the current B mode frame rate, unit is Hz
 *
 * @return FrameRateBmode
 */
-(float) libGetFrameRateBmode;

/**
 * Required to set B mode frame rate
 *
 * @param newFrameRateBmode reasonable value 4~15
 */
-(void) libSetFrameRateBmode:(float) newFrameRateBmode;

/**
 * Get all frame rates that can be set in C mode, unit is Hz
 *
 * @param p_fAllFrameRateCmode array to get framerate for C mode
 * @param nLength array number assigned to get framerate for C mode data
 */
-(int) libGetAllFrameRateCmode:(float *)p_fAllFrameRateCmode nLength:(const int) nLength;

/**
 * Get the current C mode frame rate, unit is Hz
 *
 * @return FrameRateCmode
 */
-(float) libGetFrameRateCmode;

/**
 * Required to set C mode frame rate
 *
 * @param newFrameRateCmode reasonable value 4~15
 */
-(void) libSetFrameRateCmode:(float) newFrameRateCmode;

/**
 * Get the current temperature of the image updated at half speed (when this temperature is reached, frameRate will be halved)
 *
 * @return throttleHalfTemperature
 */
-(int) libGetThrottleHalfTemperature;

/**
 * Requires setting the temperature for updating images at half speed (when this temperature is reached, frameRate will be halved)
 *
 * @param temperature ThrottleHalfTemperature
 *
 */
-(void) libSetThrottleHalfTemperature:(int) temperature;

/**
 * Obtain the temperature at which the initial inspection probe stops working
 *
 * @return InitialCheckTemperature
 */
-(int) libGetInitialCheckTemperature;

/**
 * Get the scanline currently being scanned by M mode: 0 ~ 127
 *
 * @return scanlineMmode
 */
-(int) libGetScanlineMmode;

/**
 * Requires setting of M mode scanline being scanned
 * The event ID ON_SCANLINEMMODE_SET returned by ScanListener indicates that the setting is successful.
 * The event ID ON_SCANLINEMMODE_SET_ERROR returned by ScanListener indicates that the setting failed.
 *
 * @param scanline reasonable value 0 ~ 127
 */
-(void) libSetScanlineMmode:(int) scanline;

/**
 * return the scanLine percentage of PWmode
 *
 * @return scanLine percentage of PWmode
 */
-(float) libGetScanlinePWmode;

/**
 * request to set scanLinePWmode
 *
 * @param scanLinePercentage value: 0~1
 */
-(void) libSetScanLinePWmode:(float) scanLinePercentage;

/**
 * Get FPGA version
 *
 * @return FPGA version number
 */
-(int) libGetFpgaRev;

/**
 * Get FPGA ID
 *
 * @return FPGA ID
 */
-(unsigned long long) libGetFpgaID;

/**
 * Get GetBoard  ID
 *
 * @return GetBoard ID
 */
-(unsigned int) libGetBoardID;

/**
 * Get GetProduct  ID
 *
 * @return GetProduct ID
 */
-(unsigned short) libGetProductID;

/**
 * Get GetSHA256  ID
 *
 * @return GetSHA256 ID
 */
-(unsigned long long) libGetSHA256ID;

/**
 * Get GetSpecies  ID
 *
 * @return GetSpecies ID
 */
-(unsigned short) libGetSpeciesID;

/**
 * Get GetVendorID  ID
 *
 * @return GetVendor ID
 */
-(unsigned short) libGetVendorID;

/**
 * Get Missing Golden Image
 *
 * @return GetMissingGoldenImage ture or false
 */
-(bool) libGetMissingGoldenImage;

/**
 * Get the current color persistence setting value
 *
 * @return color persistence
 * @warning deprecated. No longer used.
 */
-(int) libGetColorPersistence;

/**
 * Set persistence
 *
 * @param colorPersistence reasonable value 0 ~ 4
 * @warning deprecated. No longer used.
 */
-(void) libSetColorPersistence:(int) colorPersistence;

/**
 * Get the current color max flow speed unit cm/s
 *
 * @return color max flow speed
 */
-(float) libGetColorMaxFlowSpeed;

/**
 * Get the temperature (int) below which Probe turns off the fan.
 *
 * @return temperature Turn off fan temperature
 */
-(int) libGetFanLowerTemperature;

/**
 * Get the temperature (int) higher than Probe to turn on the fan
 *
 * @return temperature Start fan temperature
 */
-(int) libGetFanUpperTemperature;

/**
 * Obtain the initial battery power lower than the number of degrees before disconnecting
 *
 * @return batterylevel
 */
-(int) libGetInitialCheckBatteryLevel;

/**
 * Get G sensor status
 *
 * @return Gsensor: GSensorNone: 0, GSensorMotion: 1, GSensorSleep: 2
 */
-(unsigned char) libGetGSensor;

/**
 * Get convex radius
 *
 * @return r convex radius (pixel), if it is linear, return 0
 */
-(float) libGetRPx;

/**
 * Get the convex angle
 *
 * @return theta convex angle (radius), if it is linear, return 0
 */
-(float) libGetTheta;

/**
 * Get the x-axis coordinate of the convex origin
 *
 * @return originXPx convex origin x-axis coordinate (pixel), if it is linear, return 0
 */
-(float) libGetOriginXPx;

/**
 * Get the y-axis coordinate of the convex origin
 *
 * @return originYPx convex origin y-axis coordinate (pixel), if it is linear, return 0
 */
-(float) libGetOriginYPx;

/**
 * Get the largest gray map number
 *
 * @return The largest gray map number
 */
-(size_t) libGetGrayMapCount;

/**
 * Get the original width of the ultrasound image. Different depths correspond to different image widths.
 *
 * @param baseCount baseCount
 * @return ultrasound image width
 */
-(int) libGetDstWidthPx:(int) baseCount;

/**
 * Returns the width of the underlying image bitmap
 *
 * @return image bitmap width (pixels)
 */
-(int) libGetImageWidthPx;

/**
 * Return the height of the underlying image bitmap
 *
 * @return height of image bitmap (pixels)
 */
-(int) libGetImageHeightPx;

/**
 * Set linear ROI data
 *
 **********************************
 *       Ball0       Ball1
 *        _________
 *        \                   \
 *         \   ROI         \
 *          \__________\
 *         Ball3         Ball2
 **********************************
 *
 * @param roiXPx Ball0.x
 * @param roiYPx Ball0.y
 * @param roiX2Px Ball2.x
 * @param roiY2Px Ball2.y
 * @param roiAngleTan = libGetColorAngleTan()
 */
-(void) libSetLinearRoiData:(float)roiXPx ROIYpx:(float) roiYPx ROIX2px:(float)roiX2Px ROIY2px:(float)roiY2Px ROIAngleTan:(float) roiAngleTan;

/**
 * Update linear ROI location with kernel area constraint.
 * Kernel adjusts the ROI in pixel space so its area stays within the kernel
 * limit when one corner is being dragged.
 *
 * @param roiXPx Ball0.x in pixel space
 * @param roiYPx Ball0.y in pixel space
 * @param roiX2Px Ball2.x in pixel space
 * @param roiY2Px Ball2.y in pixel space
 * @param roiAngleTan libGetColorAngleTan()
 * @param movePoint which ball was dragged (0=Ball0, 1=Ball1, 2=Ball2, 3=Ball3)
 * @param outX1 in/out: Ball0.x after kernel constraint
 * @param outX2 in/out: Ball2.x after kernel constraint
 * @param outY1 in/out: Ball0.y after kernel constraint
 * @param outY2 in/out: Ball2.y after kernel constraint
 */
-(void) libUpdateLinearRoiLocation:(float)roiXPx roiYPx:(float)roiYPx roiX2Px:(float)roiX2Px roiY2Px:(float)roiY2Px roiAngleTan:(float)roiAngleTan movePoint:(int)movePoint outX1:(float*)outX1 outX2:(float*)outX2 outY1:(float*)outY1 outY2:(float*)outY2;

/**
 * Set convex ROI data
 *
 *            (getOriginXPx(), getOriginYPx())
 *                           *
 *
 *
 *                       /\______/\  ---> roiStartRPx
 *                      /          \
 *                     /   ROI      \
 *       roiStartTheta/______________\---> roiEndRPx
 *                      roiDiffTheta
 *
 *
 * @param roiStartRPx inner arc radius of ROI
 * @param roiEndRPx outer arc radius of ROI
 * @param roiStartTheta start theta of ROI
 * @param roiEndTheta = roiStartTheta + roiDiffTheta
 */
-(void) libSetConvexRoiData:(float) roiStartRPx roiEndRPx:(float) roiEndRPx roiStartTheta:(float) roiStartTheta roiEndTheta:(float) roiEndTheta;

/**
 * Update convex ROI location with kernel area constraint.
 * Kernel adjusts the ROI so its area stays within the kernel limit when one
 * corner is being dragged.
 *
 * @param roiStartRPx inner arc radius (pixel)
 * @param roiEndRPx outer arc radius (pixel)
 * @param roiStartTheta theta of right edge (kernel convention: smaller theta)
 * @param roiEndTheta theta of left edge (kernel convention: larger theta)
 * @param movePoint which ball was dragged (0=Ball0, 1=Ball1, 2=Ball2, 3=Ball3)
 * @param outStartR in/out: roiStartRPx after kernel constraint
 * @param outEndR in/out: roiEndRPx after kernel constraint
 * @param outStartTheta in/out: roiStartTheta after kernel constraint
 * @param outEndTheta in/out: roiEndTheta after kernel constraint
 */
-(void) libUpdateConvexRoiLocation:(float)roiStartRPx roiEndRPx:(float)roiEndRPx roiStartTheta:(float)roiStartTheta roiEndTheta:(float)roiEndTheta movePoint:(int)movePoint outStartR:(float*)outStartR outEndR:(float*)outEndR outStartTheta:(float*)outStartTheta outEndTheta:(float*)outEndTheta;

/**
 * get Probe Type
 *
 * @return Probe Type, e.g. Linear = 0x49; convex = 0x47
 */
-(int) libGetProbeType:(bool) isRealId;

/**
 * Get the currently set gray map number
 *
 * @return gray map number
 */
-(int) libGetGrayMap;

/**
 * Set gray map
 *
 * @param grayMap Reasonable values ​​0 ~ GrayMapMaxValue
 */
-(void) libSetGrayMap:(int) grayMap;

/**
 * Get the largest gray map number
 *
 * @return The largest gray map number
 */
-(int) libGetGrayMapMaxValue;

/**
 * Add image to cine loop
 *
 * @param img image
 * @param len image length
 * @param frameWidth image width
 *
 * @warning deprecated. No longer used.
 */
-(void) libAddCineImage:(unsigned char*) img len:(int) len frameWidth:(int) frameWidth;

/**
 * return raw image in cinebuffer
 *
 * @param index index to cinebuffer
 * @return return raw image
 */
-(unsigned char*) libGetRawImage:(int) index;

/**
 * return raw image width by index in cine buffer
 *
 * @param index index to cinebuffer
 * @return return raw image width in cinebuffer
 */
-(int) libGetCineWidth:(int) index;

/**
 * Get max frame numbers accumulated in cinebuffer
 *
 * @return MaxFrame in cinebuffer
 */
-(int) libGetMaxFrame;

/**
 * Set frame number 0 in cinebuffer
 *
 */
-(void) libResetFrame;

/**
 * Operate file IO in kernel
 *
 * @warning Never used in UI layer
 */
-(void) libPixelsIO:(int) rw buf:(unsigned char*) buf idx:(int) idx len:(int) len;

/**
 * Close all file IO pointers in the kernel
 *
 */
-(void) libPixelsIOClose;

/**
 * Get PW buffer capacity
 *
 * @return PW buffer capacity
 */
-(int) libGetPWBufferMax;

/**
 * Get PW mode image buffer size
 *
 * @return PW mode buffer Size
 */
-(int) libGetPWBufferSize;

/**
 * Clear PW mode image buffer
 *
 */
-(void) libClearPWBuffer;

/**
 * Get M buffer capacity
 *
 * @return M buffer capacity
 */
-(int) libGetMBufferMax;

/**
 * Requires setting of M mode scanline being scanned
 * The ScanListener event ID returns ON_SCANLINEMMODE_SET to indicate successful setting.
 * ON_SCANLINEMMODE_SET_ERROR returned by ScanListener event ID indicates setting failure.
 *
 * @param x Reasonable value 0 ~ 127
 */
-(void) libSetScanlineMmode_v:(int) x;

/**
 * Get B mode image
 *
 * @return B mode image
 * @warning deprecated. No longer used.
 */
-(unsigned char*) libGetRawImageB;

/**
 * Get PW image map
 *
 * @warning deprecated. Replaced by libGetPWEnvImage()
 */
-(unsigned char*) libGetPWImage;

/**
 * Get M mode images
 *
 * @return M mode image
 */
-(unsigned char*) libGetLineImage;

/**
 * Set enhancement type
 *
 * @param type enhancement type
 * @warning deprecated. No longer used
 */
-(void) libSetEnhancementType:(int) type;

/**
 * get the number of Rx samples
 *
 * @return sampleNum
 */
-(int) libGetSampleNum;

/**
 * get the percentage of focus depth
 *
 * @return depthPercentagePWmode
 */
-(float) libGetDepthPercentagePWmode;

/**
 * Reserved function
 *
 * @return GainControlCmode
 */
-(bool) libGetGainControlCmode;

/**
 * Reserved function
 *
 * @param enable should always be set to true
 */
-(void) libSetGainControlCmode:(bool) enable;

/**
 * Reserved function
 *
 * @return MagThByPwrCmode
 */
-(unsigned char) libGetMagThByPwrCmode;

/**
 * Reserved function
 *
 * @param enable should always be set to true
 */
-(void) libSetMagThByPwrCmode:(bool) enable;

/**
 * Probe element default number 128; Phase Array device: 64
 *
 * @return element
 */
-(int) libGetElement;

/**
 * Get all available scanLineBmodes
 *
 * @param p_nAllScanlinesBmode array to get scanlines for B mode
 * @param nLength array number assigned to get scanlines for B mode data
 */
-(int) libGetAllScanlinesBmode:(int *)p_nAllScanlinesBmode nLength:(const int) nLength;

/**
 * Set scanLineBmode number: 128 or 256 beam lines
 *
 * @param scanlinesBmode scanLineBmode, reasonable value: 128 or 256
 */
-(void) libSetScanlinesBmode:(int) scanlinesBmode;

/**
 * Read B mode raw image height
 *
 * @return RawDataHeight
 */
-(int) libGetRawDataHeight;

/**
 * get Beamform Method  in B mode
 *
 * 1:PlaneWave 2:Focus 3:RetroFocus 4:HarmonicFocus
 * @return Method id
 */
-(int) libGetFocusBmode;

/**
 * set Beamform Method  in B mode
 *
 * @param focusMode reasonable value: 1:PlaneWave 2:Focus 3:RetroFocus 4:HarmonicFocus
 */
-(void) libSetFocusBmode:(int) focusMode force:(bool) force;

/**
 * get VoltageLevel in B mode
 *
 * 0:None 1:Low 2:High
 * @return Voltage Level
 */
-(int) libGetVoltageLevel;

/**
 * set VoltageLevel in B mode
 *
 * @param level reasonable value: 0:None 1:Low 2:High
 */
-(void) libSetVoltageLevel:(int) level;

/**
 * get kernel API version
 *
 * @return Kernel API version
 */
-(char*) libGetProbeAPIVersion;

/**
 * get JpegEncodeBmode status in B mode
 *
 * @return true means that the image is transmitted using JPEG image compression; false means that the image is transmitted without compressing the image.
 */
-(bool) libGetJpegEncodeBmode;

/**
 * set JpegEncodeBmode status in B mode
 *
 * @param encode true means image transmission using JPEG compression; false means image transmission without compression.
 */
-(void) libSetJpegEncodeBmode:(bool) encode;

/**
 * get Jpeg compression quality in B mode
 *
 * @return value from 0 to 7
 */
-(int) libGetJpegQuality;

/**
 * set jpeg compression quality in B mode
 *
 * @param jpegQuality from 0 to 7
 */
-(void) libSetJpegQuality:(int) jpegQuality;

/**
 * set ColorInitPercentage in CF mode for ROI
 *
 * @param percentage ColorInitPercentage
 */
-(void) libSetColorInitPercentage:(float) percentage;

/**
 * Terminate kernel engine
 *
 */
-(void) libTermination;

/**
 * Load all preset info
 *
 * @return all preset info
 */
-(NSMutableArray*)getPresetOptions:(int)headIndex;

/**
 * Setting preset set to the probe according to preset ID
 *
 */
-(void)libReturnDefaultSettingParams;

/**
 * Get setting param set to the probe
 *
 * @param sid preset id
 * @return setting param
 */
-(void)libSetAndReturnSettingParams: (int)sid;

/**
 * Add new preset
 *
 * @param sid preset ID
 * @param titleName preset title name
 * @param imaName preset image name
 *
 * @return 0: failed; sid: successful
 */
-(int)sqlInsertPreset:(int)sid titleName:(NSString *)titleName imaName:(NSString *)imaName;

/**
 * Delete preset
 *
 * @param sid preset ID
 *
 * @return true: successful; false: failed
 */
-(bool)sqlDeletePreset:(int)sid;

/**
 * Save DICOM
 *
 * @param  img  ultrasound image
 * @param  len image_width * image_height
 * @param  frameWidth image width
 * @param  strlen strlen of patientDatas
 * @param  strPatientDatas the info of the ultrasound image
 * @param  idx the id of the ultrasound image(0 = the last image, used in multi-frame, set 0 in single-frame)
 * @param  max ultrasound image count (used in multi-frame)
 * @param  strPath the direct path to export .dcm (including file name and file extension)
 * @return 1 if successful; < 1 otherwise
 */
-(int)add_dicomWithImg:(UIImage*)img len:(int)len frameWidth:(int)frameWidth strlen:(int)strlen patientDatas:(NSArray*)strPatientDatas idx:(int)idx max:(int)max path:(NSString*)strPath;
/**
 * Reset DICOM kernel engine
 *
 * @param _companyName to set the company info for DICOM tag
 */
-(void)reset_dicom: (NSString*)_companyName;

/**
 * Get DICOM tag Pixel Spacing
 *
 * @param  depth  the depth of the ultrasound image
 * @param  px_width the width of the ultrasound image (pixel: getImageWidthPx)
 * @param  px_height the height of the ultrasound image (pixel: getImageHeightPx)
 * @param  scr_px_width the view width of the ultrasound image (pixel)
 * @param  scr_px_height the view height of the ultrasound image (pixel)
 *
 * @return DICOM tag Pixel Spacing
 */
-(NSString *) getDicomPixelSpacing:(float)depth px_width:(int)px_width px_height:(int)px_height scr_px_width:(int)scr_px_width scr_px_height:(int)scr_px_height;

/**
 * get DICOM Feature Type
 *
 * @return DICOM Feature Type
 */
-(BOOL) getDicomFeatureType:(NSString*)spacing;

/**
 * get Feature Type of Probe
 *
 * @return Feature Type
 */
-(int) getFeatureType;

/**
 * Set register data to the probe
 *
 * @param adr address
 * @param len data length
 * @param value data value
 *
 * @return true: successful; false: failed
 */
-(bool) libSetRegData:(int)adr len:(int) len value:(int)value;

/**
 * Get register data from the probe
 *
 * @param adr address
 * @param len data length
 *
 * @return register data: successful; 0: failed
 */
-(int) libGetRegData:(int)adr len:(int) len;

/**
 * Set B mode image mirror by new mirror method 2
 *
 *  @param mirror_on boolean to set if mirror is ON/OFF
 */
-(void) libSetMirrorBmode2: (bool) mirror_on;

/**
 * Check if B mode image mirror is ON or OFF by new mirror method 2
 *
 * @return mirror: true for ON; false for OFF
 */
-(bool) libGetMirrorBmode2;

/**
 * @brief get runtime value by mode and paramId in kernel
 *
 * pid:
 * PARAM_FREQ, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_FREQ_THI, mode available = MODE_B.
 * PARAM_FREQ_RX, mode available = MODE_B.
 * PARAM_DEPTH, mode available = MODE_B, MODE_C, MODE_PD.
 * PARAM_ENHANCEMENT, mode available = MODE_B.
 * PARAM_DR, mode available = MODE_B.
 * PARAM_GRAY_MAP, mode available = MODE_B.
 * PARAM_GAIN, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_PRF, mode available = MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_CYCLE_NUM, mode available = MODE_B, MODE_C, MODE_PW, MODE_PD.
 * PARAM_COMPOUND_NUM, mode available = MODE_B.
 * PARAM_FRAME_RATE, mode available = MODE_B, MODE_C, MODE_PD.
 * PARAM_PERSISTENCE, mode available = MODE_B.
 * PARAM_COLOR_ANGLE, mode available = MODE_C.
 * PARAM_COLOR_SENSITIVITY, mode available = MODE_C.
 * PARAM_COLOR_NOISE, mode available = MODE_C.
 * PARAM_SCAN_LINES, mode available = MODE_B.
 * PARAM_FOCUS_BMODE, mode available = MODE_B.
 * PARAM_VOLTAGE, mode available = MODE_B, MODE_C.
 * PARAM_OHM, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_WALL_FILTER, mode available = MODE_C, MODE_PW, MODE_PD.
 * PARAM_GATE_NUM_PW, mode available = MODE_PW.
 * PARAM_GATE_MM_PW, mode available = N/A
 * PARAM_GATE_ANGLE_PW, mode available = MODE_PW.
 * PARAM_DECIMATION, mode available = MODE_B, MODE_PW.
 * PARAM_ENSEMBLE_PW, mode available = MODE_PW.
 * PARAM_QUERY_SYS_FOCUS, mode available = MODE_B, MODE_C.
 * PARAM_PW_MEASURE_VTI_ENV, mode available = N/A
 * PARAM_PW_MEASURE_VTI_POINT, mode available = N/A
 * PARAM_SPATIAL_COMPOUNDING, mode available = MODE_B.
 * SYS_MENU_ADV_ON, mode available = N/A
 * SYS_PROBE_ANGLE, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_COLOR_MAP_ARRAY, mode available = N/A
 * SYS_FREEZE_TIMER, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_JPEG_ENCODE_BMODE, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_JPEG_ENCODE_QUALITY, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_CHARGE_IN_SCAN, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_FEATURE_TYPE_ARRAY, mode available = N/A
 * SYS_THROTTLE_HALF_TEMPERATURE, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_APP_STATUS_RECONNECT, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * RD_REGISTERS_LOG, mode available = N/A
 * RD_TABLE_LOG, mode available = N/A
 *
 * @param mode see #Listener.EnumMode
 * @param pid see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @return float value
 */
-(float) libGetParam:(int)mode paramId:(int)pid;

/**
 * @brief set runtime value by mode and pid to kernel
 *
 * pid:
 * PARAM_FREQ, mode available = MODE_B, MODE_C, MODE_PW, MODE_PD.
 * PARAM_FREQ_THI, mode available = N/A
 * PARAM_FREQ_RX, mode available = MODE_B.
 * PARAM_DEPTH, mode available = MODE_B, MODE_C, MODE_PD.
 * PARAM_ENHANCEMENT, mode available = MODE_B.
 * PARAM_DR, mode available = MODE_B.
 * PARAM_GRAY_MAP, mode available = MODE_B.
 * PARAM_GAIN, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_PRF, mode available = MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_CYCLE_NUM, mode available = MODE_B, MODE_C, MODE_PW, MODE_PD.
 * PARAM_COMPOUND_NUM, mode available = MODE_B.
 * PARAM_FRAME_RATE, mode available = MODE_B, MODE_C, MODE_PD.
 * PARAM_PERSISTENCE, mode available = MODE_B.
 * PARAM_COLOR_ANGLE, mode available = MODE_C.
 * PARAM_COLOR_SENSITIVITY, mode available = MODE_C.
 * PARAM_COLOR_NOISE, mode available = MODE_C.
 * PARAM_SCAN_LINES, mode available = MODE_B.
 * PARAM_FOCUS_BMODE, mode available = MODE_B.
 * PARAM_VOLTAGE, mode available = MODE_B.
 * PARAM_OHM, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_WALL_FILTER, mode available = MODE_C, MODE_PW.
 * PARAM_GATE_NUM_PW, mode available = MODE_PW.
 * PARAM_GATE_MM_PW, mode available = N/A
 * PARAM_GATE_ANGLE_PW, mode available = MODE_PW.
 * PARAM_DECIMATION, mode available = MODE_B, MODE_PW.
 * PARAM_ENSEMBLE_PW, mode available = MODE_PW.
 * PARAM_QUERY_SYS_FOCUS, mode available = MODE_B, MODE_C.
 * PARAM_PW_MEASURE_VTI_ENV, mode available = N/A
 * PARAM_PW_MEASURE_VTI_POINT, mode available = N/A
 * PARAM_SPATIAL_COMPOUNDING, mode available = MODE_B.
 * SYS_MENU_ADV_ON, mode available = MODE_B, MODE_C.
 * SYS_PROBE_ANGLE, mode available =  N/A
 * SYS_COLOR_MAP_ARRAY, mode available =  N/A
 * SYS_FREEZE_TIMER, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_JPEG_ENCODE_BMODE, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_JPEG_ENCODE_QUALITY, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_CHARGE_IN_SCAN, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_FEATURE_TYPE_ARRAY, mode available = N/A
 * SYS_THROTTLE_HALF_TEMPERATURE, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_APP_STATUS_RECONNECT, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * RD_REGISTERS_LOG, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * RD_TABLE_LOG, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 *
 * @param mode see #Listener.EnumMode
 * @param pid see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @param data set this value
 */
-(void) libSetParam:(int)mode paramId:(int)pid data:(float)data;

/**
 * @brief set runtime value by mode and pid with self-defined callbackID.
 * When setting is done, kernel will send callbackID back.
 *
 * pid:
 * PARAM_FOCUS_BMODE, mode available = MODE_B.
 * PARAM_SPATIAL_COMPOUNDING, mode available = MODE_B.
 *
 * @param mode see #Listener.EnumMode
 * @param pid see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @param data set this value into kernel by mode and pid.
 * @param cbid send this id back to "scanListener.ON_UI_EVENT_DONE" when kernel is done.
 */
-(void) libSetParamSP:(int)mode paramId:(int)pid data:(float)data callbackID:(int)cbid;

/**
 * Get scanlines from kernel by mode and beamforming mode
 *
 * @param mode  b, m, c, pw, pd
 * @param focus  focus
 *
 * @return 128 or 256 B mode scanlines
 */
-(int) libGetScanlines:(int)mode focus:(int)focus;

/**
 * @brief get available value list by mode and pid in kernel
 *
 * pid:
 * PARAM_FREQ, mode available = MODE_B, MODE_C, MODE_PW.
 * PARAM_FREQ_THI, mode available = MODE_B.
 * PARAM_FREQ_RX, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_DEPTH, mode available = MODE_B.
 * PARAM_ENHANCEMENT, mode available = MODE_B.
 * PARAM_DR, mode available = N/A
 * PARAM_GRAY_MAP, mode available = N/A
 * PARAM_GAIN, mode available = N/A
 * PARAM_PRF, mode available = MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_CYCLE_NUM, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * PARAM_COMPOUND_NUM, mode available = MODE_B.
 * PARAM_FRAME_RATE, mode available = MODE_B, MODE_C, MODE_PD.
 * PARAM_PERSISTENCE, mode available = MODE_B.
 * PARAM_COLOR_ANGLE, mode available = MODE_C.
 * PARAM_COLOR_SENSITIVITY, mode available = MODE_C.
 * PARAM_COLOR_NOISE, mode available = MODE_C.
 * PARAM_SCAN_LINES, mode available = N/A
 * PARAM_FOCUS_BMODE, mode available = N/A
 * PARAM_VOLTAGE, mode available = N/A
 * PARAM_OHM, mode available = N/A
 * PARAM_WALL_FILTER, mode available = MODE_C, MODE_PW.
 * PARAM_GATE_NUM_PW, mode available = MODE_PW.
 * PARAM_GATE_MM_PW, mode available = N/A
 * PARAM_GATE_ANGLE_PW, mode available = N/A
 * PARAM_DECIMATION, mode available = MODE_PW.
 * PARAM_ENSEMBLE_PW, mode available = MODE_PW.
 * PARAM_QUERY_SYS_FOCUS, mode available = N/A
 * PARAM_PW_MEASURE_VTI_ENV, mode available = N/A
 * PARAM_PW_MEASURE_VTI_POINT, mode available = N/A
 * PARAM_SPATIAL_COMPOUNDING, mode available = N/A
 * SYS_MENU_ADV_ON, mode available = N/A
 * SYS_PROBE_ANGLE, mode available = N/A
 * SYS_COLOR_MAP_ARRAY, mode available = MODE_B, MODE_C, MODE_PD.
 * SYS_FREEZE_TIMER, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_JPEG_ENCODE_BMODE, mode available = N/A
 * SYS_JPEG_ENCODE_QUALITY, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_CHARGE_IN_SCAN, mode available = N/A
 * SYS_FEATURE_TYPE_ARRAY, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * SYS_THROTTLE_HALF_TEMPERATURE, mode available = N/A
 * SYS_APP_STATUS_RECONNECT, mode available = MODE_B, MODE_M, MODE_C, MODE_PW, MODE_PD.
 * RD_REGISTERS_LOG, mode available = N/A
 * RD_TABLE_LOG, mode available = N/A
 *
 * @param mode see #Listener.EnumMode
 * @param pid see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @param pdata pointer of data
 * @param len array length of pdata
 */
-(void) libGetParamArray:(int)mode paramId:(int)pid data:(float*)pdata length:(int*)len;

/**
 * Get parameter array to kernel by mode and pid
 *
 * @param mode  b, m, c, pw, pd
 * @param pid parameter id
 * @param pdata pointer of data
 * @param len array length of pdata
 */
-(void) libGetParamIntArray:(int)mode paramId:(int)pid data:(int*)pdata length:(int*)len;

/**
 * Set parameter array to kernel by mode and pid
 *
 * @param mode  b, m, c, pw, pd
 * @param pid parameter id
 * @param pdata pointer of data
 * @param len array length of pdata
 */
-(void) libSetParamArray:(int)mode paramId:(int)pid data:(float* )pdata length:(int)len;

/**
 * Excute reconnection process once the Wi-Fi connection is disconnected
 *
 * @param notPreset should be always true
 *
 */
-(void) libExeReConnect:(bool) notPreset;

/**
 * Get reconnection result
 *
 * @return true if success; false if failed
 */
-(bool) libGetReconnectStatus;

/**
 * Get current Wi-Fi channel set for the probe
 *
 * @return Success: value from 1 to 11.(2.4GHz channel). >= 36(5GHz channel) Failed: 0xFFFFFFFF
 */
-(int) libGetWiFiChannel;

/**
 * Get current Wi-Fi generation set for the probe
 *
 * @return Success: value from 1 or 2
 */
-(int) libGetWiFiGeneration;

/**
 * Set Wi-Fi connection channel for the probe
 *
 * @param channel data from 1 to 11 (2.4GHz channel). >= 36(5GHz channel)
 * @return Success: true. Failed: false
 */
-(bool) libSetWiFiChannel:(int)channel;

/**
 * Get current Wi-Fi firmware version of the probe
 *
 * @return Wi-Fi version
 */
-(int) libGetWiFiVersion;

/**
 * Check if current probe supports switching Wi-Fi channel function
 *
 * @return Yes: true. No: false
 */
-(bool) libAllowSetWiFiChannel;

/**
 * Check if the kernel is under scanning mode
 *
 * @return Yes: true. No: false
 */
-(bool) libIsLive;

/**
 * Set focus point when beamform method = harmonic focus
 *
 * @param focusPercent range from 0 ~ 1.0
 */
-(void) libSetDepthPercentageFocusValue:(float) focusPercent;

/**
 * Get focus point which is current set
 *
 * @return focus point
 */
-(float) libGetDepthPercentageFocus;

/**
 * Ratio of Px / depth
 *
 * @return Ratio of Px / depth
 */
-(float) libGetRatioPx;

/**
 * Reserved function
 */
-(void) libSetDoCmodeFocus:(bool) status;

/**
 * Probe FPGA Firmware Update: Start to trigger to update FPGA
 * [FPGA update process]
 * 1. If the probe fits the update conditions, the kernel will automatically return a callback to send the "onInitializedFpgaUpdate" function in the SystemListener.
 * 2. Call "LelFpgaSpiTrigger" in the implementation of "onInitializedFpgaUpdate" function to trigger FPGA update.
 *
 * @return Trigger FPGA update Success: value 0. Failed: value < 0.
 */
-(int) libFPGA_SPI_Trigger;

/**
 * Probe FPGA Firmware Update: Get the FPGA update progress()
 * [FPGA update process] (Continue the function "LelFpgaSpiTrigger")
 * 1. If the probe meets the update conditions, the kernel will automatically return a callback to send the "onInitializedFpgaUpdate" function in the SystemListener.
 * 2. Call "LelFpgaSpiTrigger" in the implementation of "onInitializedFpgaUpdate" function to trigger FPGA update.
 * 3. After the FPGA update is triggered successfully("LelFpgaSpiTrigger" returns 0), call "LelFpgaSpiProgress" to get the update progress.
 * 4. Reboot the probe after the update is successful.("LelFpgaSpiProgress" returns 100)
 *
 * @return  FPGA Update progress: value 0~99. Update Success: value 100. Update Failed: value < 0.
 */
-(int) libFPGA_SPI_Progress;

/**
 * Probe FPGA Firmware Update: Get the FPGA version of currently connected probe
 *
 * @return FPGA version
 */
-(int) libFPGA_SPI_Current_FPGA_Rev;

/**
 * Probe FPGA Firmware Update: Get the FPGA version used for update
 *
 * @return FPGA version
 */
-(int) libFPGA_SPI_Update_FPGA_Rev;

/**
 * Probe FPGA Firmware Update: Get the WiFi FW Upgrade Result
 *
 * @return WiFi FW Upgrade Result
 */
-(int) libGetWiFiFWUpgradeResult;

/**
 * Probe FPGA Firmware Update: Get the WiFi FW Upgrade Version
 *
 * @return WiFi FW Upgrade Version
 */
-(int) libGetWiFiFWUpgradeVersion;

/**
 * Get the status of skipping FPGA update
 *
 * @return true: will skip the FPGA update; false: will not skip the FPGA update
 */
-(bool) libGetDoNotFPGAUpgrade;

/**
 * Probe FPGA Firmware Update: Skip FPGA update
 *
 * @param upgrade true: Skip FPGA Update; false: Continue to determine whether the FPGA needs to be updated
 */
-(void) libSetDoNotFPGAUpgrade: (bool) upgrade;

/**
 * Set to let the probe load all the table data when being connected next time
 */
-(void) libResetTableLoad;

/**
 * Probe FPGA Update: Check the probe status is "Golden" or not.
 * [Golden Status Definition] When the probe upgrade failed, it will load golden image file in the FPGA of the probe after rebooting next time. It will cause that the ultrasound images will not be outputted on the scan page.
 * [How to remove Golden Status] Update the probe until it succeeds and reboot the probe.
 *
 * @return Golden Status: true ; Not Golden Status: false
 */
-(bool) libFPGA_SPI_Check_Golden_Loaded;

/**
 * Get battery level raw data: 0% = 21000, 100% = 24800, 50% = 22900, 15% = 21570
 *
 * @return Battery level raw data value
 */
-(unsigned short) libGetBurninigVbat;

/**
 *Enable spatial compounding.
 *
 *@param enable true: Enable Spatial Compounding; false: Disabled Spatial Compounding.
 */
-(void) libEnableSpatialCompounding: (bool) enable;

/**
 *Get spatial compounding value.
 *
 *@return spatial compounding value
 */
-(int) libGetEnableSpatialCompounding;

/**
 *Get sum of event id
 *
 *@return (ON_SYSTEM_MAX + ON_INFO_MAX + ON_SCAN_MAX  from kernel ).
 */
-(int) libGetEventSum;

/**
 *Get sum of parameter id
 *
 *@return (ParamId_MAX + SysOptId_MAX + RDId_MAX  from kernel ).
 */
-(int) libGetParamIdMAX;

/**
 * Return real time PW mode measurement data array
 *
 * @param p_nAutoMeasure data as below:
 *     p_nAutoMeasure[0] = mv.f_VFM;
 *     p_nAutoMeasure[1] = mv.f_VFC;
 *     p_nAutoMeasure[2] = mv.f_VFCMax;
 *     p_nAutoMeasure[3] = mv.f_VFD;
 *
 * @param nLength array length to get PW mode measurement data
 *
 * @return real time PW mode measurement data array length
 */
-(float) libGetAutoMeasureVolumeValue:(float *)p_nAutoMeasure nLength:(const int) nLength diam:(float)diam;

/**
 * User Not used
 *
 *@param WiFiChannel_index
 *@param queryIndex
 *
 */
-(float) libGetTimerGlobalCalInfo: (int) WiFiChannel_index queryIndex:(int)queryIndex;

/**
 * Get battery health
 *
 *@return 0~100(%), -1 means not available
 */
-(int) libGetGaugeSOH;

/**
 * Obtain whether the probe has a license
 *
 * @param licenseIndex Type of license, matching the kernel's EnumSoftwareLicence:
 *        0 : FatMeasure; 1 : ImtBladderEf; 2 : Tdi; 3 : Elastography; 4 : TestingLicence.
 */
-(bool) libGetSoftwareLicenseAvailable: (int) licenseIndex;

/**
 * Export probe all register value
 *
 * @param strPath File path for export
 */
-(void)addPathForExportAllRegister: (NSString*)strPath;

/**
 * Load all probe info (String)
 *
 * @return all probe info
 */
-(NSMutableArray*)getStrArray: (int) strId data : (int) dataId;

/**
 * Directly set data = 0 to sound buffer of Audio Unit to remove the sound noise
 *
 */
-(void) libSetZeroSound;

/**
 * Do not directly set data = 0 to sound buffer of Audio Unit to remove the sound noise
 *
 */
- (void) libClearZeroSound;

/**
 * Trigger function in kernel by strId, str, str
 * @param strId : action id
 * @param str : data which you passed to kernel, e.g, update  by this string
 * @param val : data which you passed to kernel
 */
-(void) libSetStr: (int) strId: (NSString*) str: (int) val;

/**
 * Get String data
 *
 * @return String
 */
-(NSString*)libGetStr: (int)strId val: (int*)val;

/**
 * @brief get runtime value by mode and paramId in kernel
 *
 * paramId:
 * SYS_DUAL_PROBE_ID1, mode available = N/A
 * SYS_DUAL_PROBE_ID2, mode available = N/A
 *
 * @param mode see #Listener.EnumMode
 * @param paramId see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 */
-(int) libGetParamInt:(int)mode paramId:(int)paramId;

/**
 * @brief get runtime value by mode,  paramId and index in kernel
 *
 *
 * @param mode see #Listener.EnumMode
 * @param paramId see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @param index which parameter's index you passed to kernel
 */
-(float) libGetParamIndex:(int)mode paramId:(int)paramId index:(int)index;

/**
 * @brief set runtime value by mode, paramId and index to kernel
 *
 *
 * @param mode see #Listener.EnumMode
 * @param paramId see #CommonId.ParamId, CommonId.SysOptId, CommonId.RDId
 * @param data which you passed to kernel
 * @param index which parameter's index you passed to kernel
 */
-(void) libSetParamIndex:(int)mode paramId:(int)paramId data:(float)data index:(int)index;

@property (nonatomic,assign) id <LibManagerDelegate>  delegate;
@property (nonatomic,assign) NSString  *strAddress;
@property (nonatomic,assign) NSString  *strPort1;
@property (nonatomic,assign) NSString  *strPort2;
@end

