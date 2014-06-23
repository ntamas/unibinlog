/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef UNIBINLOG_DEBUG_H
#define UNIBINLOG_DEBUG_H

extern unsigned short int __ub_debug_mode;

#define UB_IN_DEBUG_MODE (!!__ub_debug_mode)

/**
 * Turns the debug move of \c unibinlog on or off.
 */
void ub_set_debug_mode(unsigned short int on);

/**
 * Prints a warning message to the standard error stream.
 *
 * This function behaves like printf but allows you to specify a component
 * the warning message is coming from.
 *
 * \param  component  the component the warning is coming from. May be \c NULL
 *                    if the warning is not related to a specific component.
 * \param  format     the format string.
 */
void UB_WARNING(const char* component, const char* format, ...);

#endif

