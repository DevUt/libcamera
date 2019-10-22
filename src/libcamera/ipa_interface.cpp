/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * ipa_interface.cpp - Image Processing Algorithm interface
 */

#include <ipa/ipa_interface.h>

/**
 * \file ipa_interface.h
 * \brief Image Processing Algorithm interface
 *
 * Every pipeline handler in libcamera may attach all or some of its cameras to
 * an Image Processing Algorithm (IPA) module. An IPA module is developed for a
 * specific pipeline handler and each pipeline handler may be compatible with
 * multiple IPA implementations, both open and closed source. To support this,
 * libcamera communicates with IPA modules through a standard plain C interface.
 *
 * IPA modules shall expose a public function named ipaCreate() with the
 * following prototype.
 *
 * \code{.c}
 * struct ipa_context *ipaCreate();
 * \endcode
 *
 * The ipaCreate() function creates an instance of an IPA context, which models
 * a context of execution for the IPA. IPA modules shall support creating one
 * context per camera, as required by their associated pipeline handler.
 *
 * The IPA module context operations are defined in the struct ipa_context_ops.
 * They model a low-level interface to configure the IPA, notify it of events,
 * and receive IPA actions through callbacks. An IPA module stores a pointer to
 * the operations corresponding to its context in the ipa_context::ops field.
 * That pointer is immutable for the lifetime of the context, and may differ
 * between different contexts created by the same IPA module.
 *
 * The IPA interface defines base data types and functions to exchange data. On
 * top of this, each pipeline handler is responsible for defining the set of
 * events and actions used to communicate with their IPA. These are collectively
 * referred to as IPA operations and define the pipeline handler-specific IPA
 * protocol. Each operation defines the data that it carries, and how that data
 * is encoded in the ipa_context_ops functions arguments.
 *
 * \todo Add reference to how pipelines shall document their protocol.
 *
 * IPAs can be isolated in a separate process. This implies that arguments to
 * the IPA interface functions may need to be transferred over IPC. All
 * arguments use Plain Old Data types and are documented either in the form of C
 * data types, or as a textual description of byte arrays for types that can't
 * be expressed using C data types (such as arrays of mixed data types). IPA
 * modules can thus use the C API without calling into libcamera to access the
 * data passed to the IPA context operations.
 *
 * Due to IPC, synchronous communication between pipeline handlers and IPAs can
 * be costly. For that reason, the interface operates asynchronously. This
 * implies that methods don't return a status, and that all methods may copy
 * their arguments.
 *
 * The IPAInterface class is a C++ representation of the ipa_context_ops, using
 * C++ data classes provided by libcamera. This is the API exposed to pipeline
 * handlers to communicate with IPA modules. IPA modules may use the
 * IPAInterface API internally if they want to benefit from the data and helper
 * classes offered by libcamera.
 *
 * When an IPA module is loaded directly into the libcamera process and uses
 * the IPAInterface API internally, short-circuiting the path to the
 * ipa_context_ops and back to IPAInterface is desirable. To support this, IPA
 * modules may implement the ipa_context_ops::get_interface function to return a
 * pointer to their internal IPAInterface.
 */

/**
 * \struct ipa_context
 * \brief IPA module context of execution
 *
 * This structure models a context of execution for an IPA module. It is
 * instantiated by the IPA module ipaCreate() function. IPA modules allocate
 * context instances in an implementation-defined way, contexts shall thus be
 * destroyed using the ipa_operation::destroy function only.
 *
 * The ipa_context structure provides a pointer to the IPA context operations.
 * It shall otherwise be treated as a constant black-box cookie and passed
 * unmodified to the functions defined in struct ipa_context_ops.
 *
 * IPA modules are expected to extend struct ipa_context by inheriting from it,
 * either through structure embedding to model inheritance in plain C, or
 * through C++ class inheritance. A simple example of the latter is available
 * in the IPAContextWrapper class implementation.
 *
 * \var ipa_context::ops
 * \brief The IPA context operations
 */

/**
 * \struct ipa_buffer_plane
 * \brief A plane for an ipa_buffer
 *
 * \var ipa_buffer_plane::dmabuf
 * \brief The dmabuf file descriptor for the plane
 *
 * \var ipa_buffer_plane::length
 * \brief The plane length in bytes
 */

/**
 * \struct ipa_buffer
 * \brief Buffer information for the IPA interface
 *
 * \sa libcamera::IPABuffer
 *
 * \var ipa_buffer::id
 * \brief The buffer unique ID
 *
 * \var ipa_buffer::num_planes
 * \brief The number of used planes in the ipa_buffer::planes array
 *
 * \var ipa_buffer::planes
 * \brief The buffer planes (up to 3)
 */

/**
 * \struct ipa_callback_ops
 * \brief IPA context operations as a set of function pointers
 */

/**
 * \var ipa_callback_ops::queue_frame_action
 * \brief Queue an action associated with a frame to the pipeline handler
 * \param[in] cb_ctx The callback context registered with
 * ipa_context_ops::register_callbacks
 * \param[in] frame The frame number
 *
 * \sa libcamera::IPAInterface::queueFrameAction
 */

/**
 * \struct ipa_context_ops
 * \brief IPA context operations as a set of function pointers
 *
 * To allow for isolation of IPA modules in separate processes, the functions
 * defined in the ipa_context_ops structure return only data related to the
 * libcamera side of the operations. In particular, error related to the
 * libcamera side of the IPC may be returned. Data returned by the IPA,
 * including status information, shall be provided through callbacks from the
 * IPA to libcamera.
 */

/**
 * \var ipa_context_ops::destroy
 * \brief Destroy the IPA context created by the module's ipaCreate() function
 * \param[in] ctx The IPA context
 */

/**
 * \var ipa_context_ops::get_interface
 * \brief Retrieve the IPAInterface implemented by the ipa_context (optional)
 * \param[in] ctx The IPA context
 *
 * IPA modules may implement this function to expose their internal
 * IPAInterface, if any. When implemented, libcamera may at its sole discretion
 * call it and then bypass the ipa_context_ops API by calling the IPAInterface
 * methods directly. IPA modules shall still implement and support the full
 * ipa_context_ops API.
 */

/**
 * \var ipa_context_ops::init
 * \brief Initialise the IPA context
 * \param[in] ctx The IPA context
 *
 * \sa libcamera::IPAInterface::init()
 */

/**
 * \var ipa_context_ops::register_callbacks
 * \brief Register callback operation from the IPA to the pipeline handler
 * \param[in] ctx The IPA context
 * \param[in] callback The IPA callback operations
 * \param[in] cb_ctx The callback context, passed to all callback operations
 */

/**
 * \var ipa_context_ops::configure
 * \brief Configure the IPA stream and sensor settings
 * \param[in] ctx The IPA context
 *
 * \sa libcamera::IPAInterface::configure()
 */

/**
 * \var ipa_context_ops::map_buffers
 * \brief Map buffers shared between the pipeline handler and the IPA
 * \param[in] ctx The IPA context
 * \param[in] buffers The buffers to map
 * \param[in] num_buffers The number of entries in the \a buffers array
 *
 * \sa libcamera::IPAInterface::mapBuffers()
 */

/**
 * \var ipa_context_ops::unmap_buffers
 * \brief Unmap buffers shared by the pipeline to the IPA
 * \param[in] ctx The IPA context
 * \param[in] ids The IDs of the buffers to unmap
 * \param[in] num_buffers The number of entries in the \a ids array
 *
 * \sa libcamera::IPAInterface::unmapBuffers()
 */

/**
 * \var ipa_context_ops::process_event
 * \brief Process an event from the pipeline handler
 * \param[in] ctx The IPA context
 *
 * \sa libcamera::IPAInterface::processEvent()
 */

/**
 * \fn ipaCreate()
 * \brief Entry point to the IPA modules
 *
 * This function is the entry point to the IPA modules. It is implemented by
 * every IPA module, and called by libcamera to create a new IPA context.
 *
 * \return A newly created IPA context
 */

namespace libcamera {

/**
 * \struct IPAStream
 * \brief Stream configuration for the IPA interface
 *
 * The IPAStream structure stores stream configuration parameters needed by the
 * IPAInterface::configure() method. It mirrors the StreamConfiguration class
 * that is not suitable for this purpose due to not being serialisable.
 */

/**
 * \var IPAStream::pixelFormat
 * \brief The stream pixel format
 */

/**
 * \var IPAStream::size
 * \brief The stream size in pixels
 */

/**
 * \struct IPABuffer
 * \brief Buffer information for the IPA interface
 *
 * The IPABuffer structure associates buffer memory with a unique ID. It is
 * used to map buffers to the IPA with IPAInterface::mapBuffers(), after which
 * buffers will be identified by their ID in the IPA interface.
 */

/**
 * \var IPABuffer::id
 * \brief The buffer unique ID
 *
 * Buffers mapped to the IPA are identified by numerical unique IDs. The IDs
 * are chosen by the pipeline handler to fulfil the following constraints:
 *
 * - IDs shall be positive integers different than zero
 * - IDs shall be unique among all mapped buffers
 *
 * When buffers are unmapped with IPAInterface::unmapBuffers() their IDs are
 * freed and may be reused for new buffer mappings.
 */

/**
 * \var IPABuffer::memory
 * \brief The buffer memory description
 *
 * The memory field stores the dmabuf handle and size for each plane of the
 * buffer.
 */

/**
 * \struct IPAOperationData
 * \brief Parameters for IPA operations
 *
 * The IPAOperationData structure carries parameters for the IPA operations
 * performed through the IPAInterface::processEvent() method and the
 * IPAInterface::queueFrameAction signal.
 */

/**
 * \var IPAOperationData::operation
 * \brief IPA protocol operation
 *
 * The operation field describes which operation the receiver shall perform. It
 * defines, through the IPA protocol, how the other fields of the structure are
 * interpreted. The protocol freely assigns numerical values to operations.
 */

/**
 * \var IPAOperationData::data
 * \brief Operation integer data
 *
 * The interpretation and position of different values in the array are defined
 * by the IPA protocol.
 */

/**
 * \var IPAOperationData::controls
 * \brief Operation controls data
 *
 * The interpretation and position of different values in the array are defined
 * by the IPA protocol.
 */

/**
 * \class IPAInterface
 * \brief C++ Interface for IPA implementation
 *
 * This pure virtual class defines a C++ API corresponding to the ipa_context,
 * ipa_context_ops and ipa_callback_ops API. It is used by pipeline handlers to
 * interact with IPA modules, and may be used internally in IPA modules if
 * desired to benefit from the data and helper classes provided by libcamera.
 *
 * Functions defined in the ipa_context_ops structure are mapped to IPAInterface
 * methods, while functions defined in the ipa_callback_ops are mapped to
 * IPAInterface signals. As with the C API, the IPA C++ interface uses
 * serialisable data types only. It reuses structures defined by the C API, or
 * defines corresponding classes using C++ containers when required.
 *
 * Due to process isolation all arguments to the IPAInterface methods and
 * signals may need to be transferred over IPC. The class thus uses serialisable
 * data types only. The IPA C++ interface defines custom data structures that
 * mirror core libcamera structures when the latter are not suitable, such as
 * IPAStream to carry StreamConfiguration data.
 *
 * As for the functions defined in struct ipa_context_ops, the methods defined
 * by this class shall not return data from the IPA.
 *
 * The pipeline handler shall use the IPAManager to locate a compatible
 * IPAInterface. The interface may then be used to interact with the IPA module.
 */

/**
 * \fn IPAInterface::init()
 * \brief Initialise the IPAInterface
 */

/**
 * \fn IPAInterface::configure()
 * \brief Configure the IPA stream and sensor settings
 * \param[in] streamConfig Configuration of all active streams
 * \param[in] entityControls Controls provided by the pipeline entities
 *
 * This method shall be called when the camera is started to inform the IPA of
 * the camera's streams and the sensor settings. The meaning of the numerical
 * keys in the \a streamConfig and \a entityControls maps is defined by the IPA
 * protocol.
 */

/**
 * \fn IPAInterface::mapBuffers()
 * \brief Map buffers shared between the pipeline handler and the IPA
 * \param[in] buffers List of buffers to map
 *
 * This method informs the IPA module of memory buffers set up by the pipeline
 * handler that the IPA needs to access. It provides dmabuf file handles for
 * each buffer, and associates the buffers with unique numerical IDs.
 *
 * IPAs shall map the dmabuf file handles to their address space and keep a
 * cache of the mappings, indexed by the buffer numerical IDs. The IDs are used
 * in all other IPA interface methods to refer to buffers, including the
 * unmapBuffers() method.
 *
 * All buffers that the pipeline handler wishes to share with an IPA shall be
 * mapped with this method. Buffers may be mapped all at once with a single
 * call, or mapped and unmapped dynamically at runtime, depending on the IPA
 * protocol. Regardless of the protocol, all buffers mapped at a given time
 * shall have unique numerical IDs.
 *
 * The numerical IDs have no meaning defined by the IPA interface, and IPA
 * protocols shall not give them any specific meaning either. They should be
 * treated as opaque handles by IPAs, with the only exception that ID zero is
 * invalid.
 *
 * \sa unmapBuffers()
 *
 * \todo Provide a generic implementation of mapBuffers and unmapBuffers for
 * IPAs
 */

/**
 * \fn IPAInterface::unmapBuffers()
 * \brief Unmap buffers shared by the pipeline to the IPA
 * \param[in] ids List of buffer IDs to unmap
 *
 * This method removes mappings set up with mapBuffers(). Buffers may be
 * unmapped all at once with a single call, or selectively at runtime, depending
 * on the IPA protocol. Numerical IDs of unmapped buffers may be reused when
 * mapping new buffers.
 *
 * \sa mapBuffers()
 */

/**
 * \fn IPAInterface::processEvent()
 * \brief Process an event from the pipeline handler
 * \param[in] data IPA operation data
 *
 * This operation is used by pipeline handlers to inform the IPA module of
 * events that occurred during the on-going capture operation.
 *
 * The event notified by the pipeline handler with this method is handled by the
 * IPA, which interprets the operation parameters according to the separately
 * documented IPA protocol.
 */

/**
 * \var IPAInterface::queueFrameAction
 * \brief Queue an action associated with a frame to the pipeline handler
 * \param[in] frame The frame number for the action
 * \param[in] data IPA operation data
 *
 * This signal is emitted when the IPA wishes to queue a FrameAction on the
 * pipeline. The pipeline is still responsible for the scheduling of the action
 * on its timeline.
 *
 * This signal is emitted by the IPA to queue an action to be executed by the
 * pipeline handler on a frame. The type of action is identified by the
 * \a data.operation field, as defined by the IPA protocol, and the rest of the
 * \a data is interpreted accordingly. The pipeline handler shall queue the
 * action and execute it as appropriate.
 */

} /* namespace libcamera */
