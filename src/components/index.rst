Components
==========

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   clientmanager/index
   scannermanager/index
   eloslog/index
   event/index
   eventlogging/index
   eventprocessor/index
   logger/index
   plugincontrol/index
   pluginmanager/index
   rpnfilter/index
   storagemanager/index


Component Structure
-------------------

.. code-block::

   elos/src/components/<component name>/
   ├── CMakeLists.txt
   ├── interface
   │   └── elos
   │       └── <component name>
   │           ├── defines.h
   │           └── types.h
   ├── private
   │   ├── <sub component name>
   │   │   └── <sub component name>.c
   │   └── <component name>
   │       └── <component name>.c
   └── public
       └── elos
           └── <component name>
               └── <component name>.h

interface
~~~~~~~~~

Contain header files only shared inside the elos build.

*Header-files*
*C-Source-files*

target_addinclude_dir(xyt INTERFACE ./interface/)

private
~~~~~~~

Contain the actual source files.

* Header-files are private and only used by the c-files in private.
  Use include `#include "my_private.h"`
* function in c-files have only public linkage if corresponding protoytpe is defined in a headerifle in `public`.


public
~~~~~~

Contain header files that should be packaged and deployed with elos.

- means link private-code into target which using public interface, avoid static linking

*Header-files*
*C-Source-files* : No c-Files shall be in public

define components to be installed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Build results
~~~~~~~~~~~~~

.. code-block::

   elos/build/<build type>/cmake/src/eventprocessor/
   ├── CMakeFiles
   │   └── ...
   ├── cmake_install.cmake
   ├── lib<component name>_static.a
   └── lib<component name>_static_weak.a

* lib<component name>_static.a:
  * linked into the final binary
  * target for unit tests
* lib<component name>_static_weak.a:
  * copy of the lib<component name>_static.a but with the functions symbols are marked as weak to enable mocking
  * target for unit test if mocking of internal functions is required
  * used for unit test of other components if mocking is required
* *header files to be deployed*

Unit tests
~~~~~~~~~~

Typically each c-Files get compiled as object file and collected in the
corresponding `lib<component name>_static.a`. So for each c-files a
corresponding unit test suite shall exists. See [../test/index.rst] for details
on how to write and utilise unit test.

Component Interface
~~~~~~~~~~~~~~~~~~~

Elos use a unified set of functions for a component or object.
All are optional and shall be only implemented if needed, but if needed use the
corresponding prototype below:

.. code-block::

   /**
    * Allocate memory for `sizeof(elosComponent_t)` and store the address in
    * `component*` pointer.
    * Shall call elosComponentNameInitialize.
    * If the function failed `component*` shall be unchanged.
    *
    * Parameter:
    *     component (elosComponent_t**): point to a elosComponent_t pointer, where
    *                                    to store address of the new allocated
    *                                    object. Shall be unchanged on failure.
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameNew(elosComponent_t **component);

   /**
    * Initialize the component with default values to be safely used.
    *
    * Parameter:
    *     component (elosComponent_t*): point to a elosComponent_t, which shall be
    *                                   initialized with defaults.
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameInitialize(elosComponent_t *component);

   /**
    * Copy the memory contents to a elosComponent_t from a source elosComponent_t
    * and also make a deep copy each member.
    *
    * Parameter:
    *     to (elosComponent_t*): pointer to the destination component, on failure
    *                            the state of  elosComponent_t shall be unchanged.
    *     from (elosComponent_t*): pointer to the source component
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameDeepCopy(elosComponent_t *to, elosComponent_t const *from);

   /**
    * Allocate new memory for a elosComponent_t and make a shallow copy of the
    * memory contents of the source elosComponent_t.
    *
    * Parameter:
    *     to (elosComponent_t*): pointer to the destination component pointer, on
    *                            failure the state of  elosComponent_t shall be
    *                            unchanged.
    *     from (elosComponent_t*): pointer to the source component
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameClone(elosComponent_t **to, elosComponent_t const *from);

   /**
    * Shall cleanup and free all members of elosComponent_t.
    *
    * Parameter:
    *     component (elosComponent_t*): pointer to a elosComponent_t, of which all
    *                                   memebers shall be freed.
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameDeleteMembers(elosComponent_t *component);

   /**
    * Shall free elosComponent_t itself.
    *
    * Parameter:
    *     component (elosComponent_t*): pointer to a elosComponent_t, which shall
    *                                   be freed.
    *
    * Returns:
    *     safuResultE_t:
    *         SAFU_RESULT_OK – on success.
    *         SAFU_RESULT_FAILURE – on failure.
    */
   safuResultE_t elosComponentNameDelete(elosComponent_t *component);




Build process in detail
~~~~~~~~~~~~~~~~~~~~~~~

1. compile all c-files and collect in static component library -> component ==> static library
2. create weak component library









