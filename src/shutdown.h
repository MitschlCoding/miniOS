#ifndef SHUTDOWN_H
#define SHUTDOWN_H

/**
 * @file shutdown.h
 * @brief Header file for the shutdown functionality of the miniOS kernel.
 *
 * This header defines the function to shut down the system, specifically
 * for QEMU environments.
 */

/**
 * @brief Shuts down the system.
 *
 * This function performs the necessary steps to shut down the system
 * in a QEMU environment.
 */
void systemShutdown();

#endif
