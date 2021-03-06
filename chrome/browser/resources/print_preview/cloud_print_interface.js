// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.exportPath('cloudprint');

/**
 * Event types dispatched by the cloudprint interface.
 * @enum {string}
 */
cloudprint.CloudPrintInterfaceEventType = {
  INVITES_DONE: 'cloudprint.CloudPrintInterface.INVITES_DONE',
  INVITES_FAILED: 'cloudprint.CloudPrintInterface.INVITES_FAILED',
  PRINTER_DONE: 'cloudprint.CloudPrintInterface.PRINTER_DONE',
  PRINTER_FAILED: 'cloudprint.CloudPrintInterface.PRINTER_FAILED',
  PROCESS_INVITE_DONE: 'cloudprint.CloudPrintInterface.PROCESS_INVITE_DONE',
  PROCESS_INVITE_FAILED: 'cloudprint.CloudPrintInterface.PROCESS_INVITE_FAILED',
  SEARCH_DONE: 'cloudprint.CloudPrintInterface.SEARCH_DONE',
  SEARCH_FAILED: 'cloudprint.CloudPrintInterface.SEARCH_FAILED',
  SUBMIT_DONE: 'cloudprint.CloudPrintInterface.SUBMIT_DONE',
  SUBMIT_FAILED: 'cloudprint.CloudPrintInterface.SUBMIT_FAILED',
};

/**
 * @typedef {{
 *   user: string,
 *   origin: !print_preview.DestinationOrigin,
 *   printers: (!Array<!print_preview.Destination>|undefined)
 * }}
 */
cloudprint.CloudPrintInterfaceSearchDoneEvent;

/**
 * @typedef {{
 *   printer: !print_preview.Destination,
 * }}
 */
cloudprint.CloudPrintInterfacePrinterDoneEvent;

/**
 * @typedef {{
 *   destinationId: string,
 *   destinationOrigin: !print_preview.DestinationOrigin,
 * }}
 */
cloudprint.CloudPrintInterfacePrinterFailedEvent;

/**
 * @typedef {{
 *   invitations: !Array<!print_preview.Invitation>,
 *   user: string,
 * }}
 */
cloudprint.CloudPrintInterfaceInvitesDoneEvent;

/**
 * @typedef {{
 *   user: string,
 * }}
 */
cloudprint.CloudPrintInterfaceInvitesFailedEvent;

/**
 * @typedef {{
 *   invitation: !print_preview.Invitation,
 * }}
 */
cloudprint.CloudPrintInterfaceProcessInviteEvent;

cr.define('cloudprint', function() {
  /** @interface */
  class CloudPrintInterface {
    /**
     * @return {boolean} Whether a search for cloud destinations is in progress.
     */
    isCloudDestinationSearchInProgress() {}

    /** @return {!cr.EventTarget} The event target for this interface. */
    getEventTarget() {}

    /**
     * Sends Google Cloud Print search API request.
     * @param {?string=} opt_account Account the search is sent for. When
     *      null or omitted, the search is done on behalf of the primary user.
     * @param {print_preview.DestinationOrigin=} opt_origin When specified,
     *     searches destinations for {@code opt_origin} only, otherwise starts
     *     searches for all origins.
     */
    search(opt_account, opt_origin) {}

    /**
     * Sends Google Cloud Print printer sharing invitations API requests.
     * @param {string} account Account the request is sent for.
     */
    invites(account) {}

    /**
     * Accepts or rejects printer sharing invitation.
     * @param {!print_preview.Invitation} invitation Invitation to process.
     * @param {boolean} accept Whether to accept this invitation.
     */
    processInvite(invitation, accept) {}

    /**
     * Sends a Google Cloud Print submit API request.
     * @param {!print_preview.Destination} destination Cloud destination to
     *     print to.
     * @param {string} printTicket The print ticket to print.
     * @param {!print_preview.DocumentInfo} documentInfo Document data model.
     * @param {string} data Base64 encoded data of the document.
     */
    submit(destination, printTicket, documentInfo, data) {}

    /**
     * Sends a Google Cloud Print printer API request.
     * @param {string} printerId ID of the printer to lookup.
     * @param {!print_preview.DestinationOrigin} origin Origin of the printer.
     * @param {string=} account Account this printer is registered for. When
     *     provided for COOKIES {@code origin}, and users sessions are still not
     *     known, will be checked against the response (both success and failure
     *     to get printer) and, if the active user account is not the one
     *     requested, {@code account} is activated and printer request reissued.
     */
    printer(printerId, origin, account) {}
  }

  // Export
  return {
    CloudPrintInterface: CloudPrintInterface,
  };
});
