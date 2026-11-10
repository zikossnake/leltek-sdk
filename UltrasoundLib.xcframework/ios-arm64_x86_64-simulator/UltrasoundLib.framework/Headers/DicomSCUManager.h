//
//  DicomSCUManager.h
//  Ultrasound
//
//  Created by gsh_mac_2018 on 2020/4/29.
//  Copyright © 2020 Leltek Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface DicomSCUManager : NSObject

+(instancetype)shared;

/**
 * Initialize DICOMSCU kernel engine
 *
 * @param ip Peer IP address
 * @param port Peer port number
 * @param peer_AE_TITLE Peer AE title
 * @param app_AE_TITLE Application AE title
 *
 * @return 1 if successfull; otherwise failed with error code
 */
-(int) resetWithIP:(NSString*)ip Peer_AE_TITLE:(NSString*)peer_AE_TITLE App_AE_TITLE:(NSString*)app_AE_TITLE port:(unsigned short)port;

/**
 * Test C-Find for querying worklist
 *
 *@param filters query condition
 * query_condition[0] = PatientID;
 * query_condition[1] = Patient Name;
 * query_condition[2] = DOB;
 * query_condition[3] = Accession No;
 * query_condition[4] = StartDate;
 * query_condition[5] = EndDate;
 * query_condition[6] = Modality
 *
 * @return 1 if successfull; otherwise failed with error code
 */
-(int) sendFindRequestWithFilter:(NSArray*)filters;

/**
 * Get num's patient data of total number of patients downloaded from worklist
 *
 * @param num num's patient
 *
 * @return Patient's data
 */
-(NSMutableArray*) queryDataWithNum:(int)num;

/**
 * Test C-EHCO function
 *
 * @return 1 if successfull; otherwise failed with error code
 */
-(int) testServer;

/**
 * Reserved function
 *
 */
-(int) sendDicomTest;

/**
 * Send DICOM file to server
 *
 * @param path DICOM file and its path
 *
 * @return 1 if successfull; otherwise failed with error code
 */
-(int) sendDicomWithPath:(NSString*)path;

/**
 * Reserved function
 *
 */
-(NSString*) getSendDicomError;

/**
 * Terminate DICOMSCU kernel engine
 */
-(void) closeAssociation;
@end

NS_ASSUME_NONNULL_END
