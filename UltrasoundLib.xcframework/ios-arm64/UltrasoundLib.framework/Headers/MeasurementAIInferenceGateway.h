//
//  MeasurementAIInferenceGateway.h
//  Ultrasound
//
//  Copyright © 2026 Leltek Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface NativeMeasurementAIResult : NSObject
@property (nonatomic, strong) UIImage *outputImage;
@property (nonatomic, strong) UIImage *outputImage2;
@property (nonatomic, assign) int colUp;
@property (nonatomic, assign) int rowUp;
@property (nonatomic, assign) int colDown;
@property (nonatomic, assign) int rowDown;
@property (nonatomic, assign) int colUp2;
@property (nonatomic, assign) int rowUp2;
@property (nonatomic, assign) int colDown2;
@property (nonatomic, assign) int rowDown2;
@property (nonatomic, copy) NSString *objectOne;
@property (nonatomic, copy) NSString *objectTwo;
@property (nonatomic, assign) int diastoleCineIdx;
@property (nonatomic, assign) int systoleCineIdx;
@end

@interface MeasurementAIInferenceGateway : NSObject
+ (nullable NativeMeasurementAIResult *)inferenceWithImage:(nullable UIImage *)inputImage
                                                   mission:(NSString *)mission;
@end

NS_ASSUME_NONNULL_END
