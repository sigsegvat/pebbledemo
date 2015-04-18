#include <pebble.h>

Window *window;
TextLayer *layer_s, *layer_h, *layer_m;
ClickConfigProvider click_config;
TickHandler tick_handler;
bool toggle = true;



void setup_textlayer(TextLayer *thelayer){
	text_layer_set_text_alignment(thelayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(thelayer));
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	layer_s = text_layer_create(GRect(0, 90, 144, 44));
  layer_m = text_layer_create(GRect(0, 50, 144, 44));
  layer_h = text_layer_create(GRect(0, 10, 144, 44));
	
  text_layer_set_font(layer_h, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_font(layer_m, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_font(layer_s, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  
	// Set the text, font, and text alignment
  setup_textlayer(layer_s);
  setup_textlayer(layer_m);
  setup_textlayer(layer_h);

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
  
  window_set_click_config_provider(window, click_config);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

void time_convert(char buf[],int unit){
  
  int s_dec = unit/10;
  int s_min = unit - s_dec*10;
  buf[0] = '0' + s_dec;
  buf[1] = '0' + s_min;
}

void tock(struct tm *tick_time, TimeUnits units_changed){

  static char s_buffer[3],m_buffer[3],h_buffer[3];
  
  time_convert(s_buffer,(*tick_time).tm_sec);
  time_convert(m_buffer,(*tick_time).tm_min);
  time_convert(h_buffer,(*tick_time).tm_hour);
  
  s_buffer[2] = m_buffer[2] = h_buffer[2] = 0;
  
  text_layer_set_text(layer_h, h_buffer);
  text_layer_set_text(layer_m, m_buffer);
  text_layer_set_text(layer_s, s_buffer);
 

}



void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  light_enable(toggle);
  toggle=!toggle;
  text_layer_set_text(layer_m, "OUT");
  text_layer_set_text(layer_h, "Time");
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
}


void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(layer_s);
	text_layer_destroy(layer_h);
  text_layer_destroy(layer_m);
  
	// Destroy the window
	window_destroy(window);
}

int main(void) {
  click_config = &click_config_provider;
  tick_handler = &tock;
	handle_init();
	app_event_loop();
	handle_deinit();
}
