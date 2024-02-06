Architecture Design Records
===========================

During the development of elos, many decisions have to be made. This chapter
provides a place to collect new ideas and discuss them until we are able to
agree on a final decision of an topic.


An *Architecture Design Record* (**ADR**) should follow roughly the pattern

* Problem Description
* Influencing factors
* Assumptions
* Considered Alternatives
* Decision
  * Rational
  * Open Points

There is an ADR-Template :doc:`ADR_template` which can be started with.


List of ADRs
------------

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   ./ADR_template
   ./atomic_variables
   ./elos-coredump
   ./elos_event_logging_concept
   ./elos_logging_frameworks
   ./event_authorization
   ./event_authorization_blacklisting
   ./event_logging_storage_concept
   ./event_retention_policies
   ./event_storage_backends
   ./event_storage_class
   ./event_throtteling
   ./fetch-features
   ./handle_kernel_module_dependencies
   ./package_registry
   ./shmem_locking
   ./usage_of_FORTIFY_SOURCE
   ./communication_protocol
   ./libelos_logger_concept

