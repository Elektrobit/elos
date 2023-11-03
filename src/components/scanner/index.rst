Scanner
=======

-  A Scanner:

   -  Is a shared object that provides the Scanner-API by exporting the
      function symbols defined in scanner.h
   -  Intended to publish messages for arbitrary events
   -  Are not explicitly limited in number, but by system resources like
      open file descriptors
   -  Is expected to achieve lower latency then publishing messages
      through tcp/ip

build’in scanner (examples)
---------------------------

-  See `Scanner-Plugins <../../plugins/scanner>`__

Scanner-API
-----------

The ScannerManager loads and initializes a Scanner. After successful
initialization the ScannerManager calls the scannerRun method in a
dedicated thread. If the ScannerManager is asked to stop all or a
particular Scanner, it will signal it to the corresponding Scanner and
join the Scanner threads.

The following diagram demonstrates the live cycle of a scanner.

.. figure:: /doc/images/scanner_sequence_diagram.png
   :alt: Scanner sequence diagram

   Scanner sequence diagram

Howto implement a scanner
~~~~~~~~~~~~~~~~~~~~~~~~~

To implement a scanner simply extend the following code snippets or take
one of the build’in scanner source as an example.

-  Create a file example_scanner.c and with the following content:

.. code:: c

   #include <stdio.h>

   #include "elos/scanner/scanner.h"

   elosScannerResultE_t elosScannerInitialize(elosScannerSession_t *session, UNUSED const elosScannerParam_t *param) {
       printf("[my scanner] is initializied\n");
       return SCANNER_OK;
   }

   elosScannerResultE_t elosScannerRun(elosScannerSession_t *session) {
       printf("[my scanner] is runnig\n");
       printf("[my scanner] has finished\n");
       return SCANNER_OK;
   }

   elosScannerResultE_t elosScannerStop(elosScannerSession_t *session) {
       printf("[my scanner] stop requested\n");
       return SCANNER_OK;
   }

   elosScannerResultE_t elosScannerFree(elosScannerSession_t *session) {
       printf("[my scanner] gets freed\n");
       free(session->context);
       return SCANNER_OK;
   }

-  compile it with :

::

   gcc --shared -o example_scanner.so example_scanner.c -I elos/src/scanner/include/

-  copy / install example_scanner.so to ELOS_SCANNER_PATH

::

   cp testscanner/example_scanner.so $ELOS_SCANNER_PATH

-  start elos and have a look for the log messages.
-  extend to your use case
