/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief Device mode driver for the library USB Audio 1.0 Class driver.
 *
 *  Device mode driver for the library USB Audio 1.0 Class driver.
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB module driver
 *        dispatch header located in LUFA/Drivers/USB.h.
 */

/** \ingroup Group_USBClassAudio
 *  \defgroup Group_USBClassAudioDevice Audio 1.0 Class Device Mode Driver
 *
 *  \section Sec_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - LUFA/Drivers/USB/Class/Device/Audio.c <i>(Makefile source module name: LUFA_SRC_USBCLASS)</i>
 *
 *  \section Sec_ModDescription Module Description
 *  Device Mode USB Class driver framework interface, for the Audio 1.0 USB Class driver.
 *
 *  @{
 */

#ifndef _AUDIO_CLASS_DEVICE_H_
#define _AUDIO_CLASS_DEVICE_H_

	/* Includes: */
		#include "../../USB.h"
		#include "../Common/Audio.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_AUDIO_DRIVER)
			#error Do not include this file directly. Include LUFA/Drivers/USB.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Type Defines: */
			/** \brief Audio Class Device Mode Configuration and State Structure.
			 *
			 *  Class state structure. An instance of this structure should be made for each Audio interface
			 *  within the user application, and passed to each of the Audio class driver functions as the
			 *  \c AudioInterfaceInfo parameter. This stores each Audio interface's configuration and state information.
			 */
			typedef struct
			{
				const struct
				{
					uint8_t  StreamingInterfaceNumber; /**< Index of the Audio Streaming interface within the device this
														*   structure controls.
														*/

					uint8_t  DataINEndpointNumber; /**< Endpoint number of the incoming Audio Streaming data, if available
													*   (zero if unused).
													*/
					uint16_t DataINEndpointSize; /**< Size in bytes of the incoming Audio Streaming data endpoint, if available
												  *   (zero if unused).
												  */

					uint8_t  DataOUTEndpointNumber; /**< Endpoint number of the outgoing Audio Streaming data, if available
													 *   (zero if unused).
													 */
					uint16_t DataOUTEndpointSize; /**< Size in bytes of the outgoing Audio Streaming data endpoint, if available
												   *   (zero if unused).
												   */
				} Config; /**< Config data for the USB class interface within the device. All elements in this section
				           *   <b>must</b> be set or the interface will fail to enumerate and operate correctly.
				           */
				struct
				{
					bool InterfaceEnabled; /**< Set and cleared by the class driver to indicate if the host has enabled the streaming endpoints
					                        *   of the Audio Streaming interface.
					                        */
				} State; /**< State data for the USB class interface within the device. All elements in this section
				          *   are reset to their defaults when the interface is enumerated.
				          */
			} USB_ClassInfo_Audio_Device_t;

		/* Function Prototypes: */
			/** Configures the endpoints of a given Audio interface, ready for use. This should be linked to the library
			 *  \ref EVENT_USB_Device_ConfigurationChanged() event so that the endpoints are configured when the configuration containing the
			 *  given Audio interface is selected.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return Boolean \c true if the endpoints were successfully configured, \c false otherwise.
			 */
			bool Audio_Device_ConfigureEndpoints(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo) ATTR_NON_NULL_PTR_ARG(1);

			/** Processes incoming control requests from the host, that are directed to the given Audio class interface. This should be
			 *  linked to the library \ref EVENT_USB_Device_ControlRequest() event.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 */
			void Audio_Device_ProcessControlRequest(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo) ATTR_NON_NULL_PTR_ARG(1);
			
			/** Audio class driver callback for the setting and retrieval of streaming endpoint properties. This callback must be implemented
			 *  in the user application to handle property manipulations on streaming audio endpoints.
			 *
			 *  When the DataLength parameter is NULL, this callback should only indicate whether the specified operation is valid for
			 *  the given endpoint index, and should return as fast as possible. When non-NULL, this value may be altered for GET operations
			 *  to indicate the size of the retreived data.
			 *
			 *  \note The length of the retrieved data stored into the Data buffer on GET operations should not exceed the initial value
			 *        of the \c DataLength parameter.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *  \param[in]     EndpointProperty    Property of the endpoint to get or set, a value from \ref Audio_ClassRequests_t.
			 *  \param[in]     EndpointAddress     Address of the streaming endpoint whose property is being referenced.
			 *  \param[in]     EndpointControl     Parameter of the endpoint to get or set, a value from \ref Audio_EndpointControls_t.
			 *  \param[in,out] DataLength          For SET operations, the length of the parameter data to set. For GET operations, the maximum
			 *                                     length of the retrieved data. When NULL, the function should return whether the given property
			 *                                     and parameter is valid for the requested endpoint without reading or modifying the Data buffer.
			 *  \param[in,out] Data                Pointer to a location where the parameter data is stored for SET operations, or where
			 *                                     the retrieved data is to be stored for GET operations.
			 *
			 *  \return Boolean true if the property get/set was successful, false otherwise
			 */
			bool CALLBACK_Audio_Device_GetSetEndpointProperty(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                                  const uint8_t EndpointProperty,
			                                                  const uint8_t EndpointAddress,
			                                                  const uint8_t EndpointControl,
			                                                  uint16_t* const DataLength,
			                                                  uint8_t* Data);

			/** Audio class driver event for an Audio Stream start/stop change. This event fires each time the device receives a stream enable or
			 *  disable control request from the host, to start and stop the audio stream. The current state of the stream can be determined by the
			 *  State.InterfaceEnabled value inside the Audio interface structure passed as a parameter.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 */
			void EVENT_Audio_Device_StreamStartStop(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo);

		/* Inline Functions: */
			/** General management task for a given Audio class interface, required for the correct operation of the interface. This should
			 *  be called frequently in the main program loop, before the master USB management task \ref USB_USBTask().
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 */
			static inline void Audio_Device_USBTask(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                        ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline void Audio_Device_USBTask(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				(void)AudioInterfaceInfo;
			}

			/** Determines if the given audio interface is ready for a sample to be read from it, and selects the streaming
			 *  OUT endpoint ready for reading.
			 *
			 *  \pre This function must only be called when the Device state machine is in the \ref DEVICE_STATE_Configured state or
			 *       the call will fail.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return Boolean \c true if the given Audio interface has a sample to be read, \c false otherwise.
			 */
			static inline bool Audio_Device_IsSampleReceived(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                                 ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline bool Audio_Device_IsSampleReceived(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				if ((USB_DeviceState != DEVICE_STATE_Configured) || !(AudioInterfaceInfo->State.InterfaceEnabled))
				  return false;

				Endpoint_SelectEndpoint(AudioInterfaceInfo->Config.DataOUTEndpointNumber);
				return Endpoint_IsOUTReceived();
			}

			/** Determines if the given audio interface is ready to accept the next sample to be written to it, and selects
			 *  the streaming IN endpoint ready for writing.
			 *
			 *  \pre This function must only be called when the Device state machine is in the \ref DEVICE_STATE_Configured state or
			 *       the call will fail.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return Boolean \c true if the given Audio interface is ready to accept the next sample, \c false otherwise.
			 */
			static inline bool Audio_Device_IsReadyForNextSample(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                                     ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline bool Audio_Device_IsReadyForNextSample(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				if ((USB_DeviceState != DEVICE_STATE_Configured) || !(AudioInterfaceInfo->State.InterfaceEnabled))
				  return false;

				Endpoint_SelectEndpoint(AudioInterfaceInfo->Config.DataINEndpointNumber);
				return Endpoint_IsINReady();
			}

			/** Reads the next 8-bit audio sample from the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsSampleReceived() function to ensure
			 *       that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return  Signed 8-bit audio sample from the audio interface.
			 */
			static inline int8_t Audio_Device_ReadSample8(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                              ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline int8_t Audio_Device_ReadSample8(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				int8_t Sample;

				(void)AudioInterfaceInfo;

				Sample = Endpoint_Read_8();

				if (!(Endpoint_BytesInEndpoint()))
				  Endpoint_ClearOUT();

				return Sample;
			}

			/** Reads the next 16-bit audio sample from the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsSampleReceived() function to ensure
			 *       that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return  Signed 16-bit audio sample from the audio interface.
			 */
			static inline int16_t Audio_Device_ReadSample16(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                                ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline int16_t Audio_Device_ReadSample16(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				int16_t Sample;

				(void)AudioInterfaceInfo;

				Sample = (int16_t)Endpoint_Read_16_LE();

				if (!(Endpoint_BytesInEndpoint()))
				  Endpoint_ClearOUT();

				return Sample;
			}

			/** Reads the next 24-bit audio sample from the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsSampleReceived() function to ensure
			 *       that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *
			 *  \return Signed 24-bit audio sample from the audio interface.
			 */
			static inline int32_t Audio_Device_ReadSample24(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			                                                ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline int32_t Audio_Device_ReadSample24(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
			{
				int32_t Sample;

				(void)AudioInterfaceInfo;

				Sample = (((uint32_t)Endpoint_Read_8() << 16) | Endpoint_Read_16_LE());

				if (!(Endpoint_BytesInEndpoint()))
				  Endpoint_ClearOUT();

				return Sample;
			}

			/** Writes the next 8-bit audio sample to the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsReadyForNextSample() function to
			 *       ensure that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *  \param[in]     Sample              Signed 8-bit audio sample.
			 */
			static inline void Audio_Device_WriteSample8(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                             const int8_t Sample) ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline void Audio_Device_WriteSample8(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                             const int8_t Sample)
			{
				Endpoint_Write_8(Sample);

				if (Endpoint_BytesInEndpoint() == AudioInterfaceInfo->Config.DataINEndpointSize)
				  Endpoint_ClearIN();
			}

			/** Writes the next 16-bit audio sample to the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsReadyForNextSample() function to
			 *       ensure that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *  \param[in]     Sample              Signed 16-bit audio sample.
			 */
			static inline void Audio_Device_WriteSample16(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                              const int16_t Sample) ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline void Audio_Device_WriteSample16(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                              const int16_t Sample)
			{
				Endpoint_Write_16_LE(Sample);

				if (Endpoint_BytesInEndpoint() == AudioInterfaceInfo->Config.DataINEndpointSize)
				  Endpoint_ClearIN();
			}

			/** Writes the next 24-bit audio sample to the current audio interface.
			 *
			 *  \pre This should be preceded immediately by a call to the \ref Audio_Device_IsReadyForNextSample() function to
			 *       ensure that the correct endpoint is selected and ready for data.
			 *
			 *  \param[in,out] AudioInterfaceInfo  Pointer to a structure containing an Audio Class configuration and state.
			 *  \param[in]     Sample              Signed 24-bit audio sample.
			 */
			static inline void Audio_Device_WriteSample24(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                              const int32_t Sample) ATTR_NON_NULL_PTR_ARG(1) ATTR_ALWAYS_INLINE;
			static inline void Audio_Device_WriteSample24(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo,
			                                              const int32_t Sample)
			{
				Endpoint_Write_16_LE(Sample);
				Endpoint_Write_8(Sample >> 16);

				if (Endpoint_BytesInEndpoint() == AudioInterfaceInfo->Config.DataINEndpointSize)
				  Endpoint_ClearIN();
			}

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Function Prototypes: */
			#if defined(__INCLUDE_FROM_AUDIO_DEVICE_C)
				void Audio_Device_Event_Stub(void) ATTR_CONST;
				
				void EVENT_Audio_Device_StreamStartStop(USB_ClassInfo_Audio_Device_t* const AudioInterfaceInfo)
				                                        ATTR_WEAK ATTR_NON_NULL_PTR_ARG(1) ATTR_ALIAS(Audio_Device_Event_Stub);
			#endif

	#endif	

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

