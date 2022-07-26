/**@file app.hpp
 * @brief function prototypes for application driver
 *
 * This contains the prototypes for the application driver which is resposible
 * for initialising, running and shutting down the main application.
 *
 */

#ifndef EM_APP_H
#define EM_APP_H

namespace application
{

/**@brief check and initialize the application
 *
 * @param width of the logical screen
 * @param height of the logical screen
 * @return true if the app was initialized properly, false otherwise
 */
bool init_app (const unsigned int width, const unsigned int height);

/**@brief launch the aplication and main loop
 *
 * @return Void
 */
void run_app ();

/**@brief Shut down the application
 *
 * @return Void
 */
void shut_down_app ();

}

#endif /* EM_APP_H */
