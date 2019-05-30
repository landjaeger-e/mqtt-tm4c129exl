#ifndef DISPLAY_BOOSTERPACKUI_H_
#define DISPLAY_BOOSTERPACKUI_H_

/** @brief Run-time Function
 *
 *This function displays the static content on the Subscriber
 *
 */
void staticcontentboosterpackUISubscribe();

/** @brief Run-time Function
 *
 *This function displays the dynamic content "CoreTemperature" (own Temperature) on the Subscriber
 *
 */
void dynamiccontentboosterpackUICoreTemperature();

/** @brief Run-time Function
 *
 *This function displays the dynamic content "DeviceName" (subscribed deviceName from pubilsher) on the Subscriber
 *
 *@param *deviceName: consists of messagebuffer of of subscribed topic: deviceName
 */
void dynamiccontentboosterpackUIPublisherDeviceName(char* deviceName);

/** @brief Run-time Function
 *
 *This function displays the dynamic content "coreTemperature" (subscribed Temp. from pubilsher) on the Subscriber
 *
 *@param *temp: consists of messagebuffer of of subscribed topic: coreTemperature
 */
void dynamiccontentboosterpackUIPublisherCoreTemeperature(char* temp);

/** @brief Run-time Function
 *
 *This function displays the dynamic content "AmbLight" (subscribed ambLight. from pubilsher) on the Subscriber
 *
 *@param *ambLight: consists of messagebuffer of of subscribed topic: ambLight
 */
void dynamiccontentboosterpackUIPublisherAmbientLight(char* ambLight);

/** @brief Run-time Function
 *
 *This function displays the static content on the Publisher
 *
 */
void staticcontentboosterpackUIPublish();

/** @brief Run-time Function
 *
 *This function displays the dynamic content "CoreTemperature" (own value) on the Publisher
 */
void dynamiccontentboosterpackUIPublishTemperature();

/** @brief Run-time Function
 *
 *This function displays the dynamic content "ambientLight" (own value) on the Publisher
 */
void dynamiccontentboosterpackUIPublishAmbientLight();

#endif /* DISPLAY_BOOSTERPACKUI_H_ */
