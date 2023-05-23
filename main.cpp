//
//  xg_main.hpp
//  life
//
//  Created by Daher Alfawares on 1/1/2019.
//  Copyright © 2019 Daher Alfawares. All rights reserved.
//

#include "xg.hpp"

#include <cmath>

using xg::v;

namespace cfg
{
    const auto screen_size = xg::vec2(1200, 800);
    const auto big_font_size = 24;
    const auto big_font_spacing = 28.0f;
    const auto big_font_name = "resources/Sansation-Regular.ttf";
    const auto small_font_size = 14;
    const auto small_font_name = "resources/Sansation-Light.ttf";
    const auto small_font_spacing = 24.0f;
    const auto tick_size = xg::vec2(0, 10);
    const auto block_size = xg::vec2(80.0f, 80.0f)*2.80f;
    const auto block_offset = xg::vec2(0, 0);
    const auto block_disabled_color = xg::color::black();
    const auto hud_block_size = xg::vec2(20.0f, 20.0f);
    const auto pitch_grid_block_size = xg::vec2(40.0f, 40.0f);
    const auto pitch_grid_origin = xg::vec2{ 250,screen_size.y - 250 } -cfg::pitch_grid_block_size*2.5f;
    const auto icon_size = xg::vec2(100.0f, 100.0f)*1.0f;
    const auto icon_offset = xg::vec2(0, -150.0f);
    const auto legend_origin = screen_size * xg::vec2(1, 0) + xg::vec2(-300.0f, 240.0f);
    const auto color_legend_origin = screen_size - xg::v{ -10.0f,60.0f };
    const auto background_color = xg::color(30, 30, 30, 255);
    const auto grid_color = xg::color::dark_gray();
    const auto note_line_shade_factor = 0.40f;
}

class stats
{
    xg::line line;
public:
    void render(xg::renderer& renderer, const xg::time& time)
        {
        xg::text text(renderer, cfg::big_font_name, cfg::big_font_size);
        xg::grid text_grid = xg::grid().start_at(cfg::screen_size * v{ 1, 1 }).space_by(v{ 0, 80 });
        text.align = xg::bottom | xg::center_h;
        text.origin = text_grid--;
        text.color = xg::color::white();

        renderer.set_color(text.color);
        text.render(renderer, time_string(time));

        line.p0 = text_grid--;
        line.p1 = text_grid--;
        line.render(renderer);
        }

    std::string time_string(const xg::time& time)
        {
        int T = int(time.seconds());
        int hours = (T) / (60 * 60);
        int minutes = (T - (hours * 60 * 60)) / 60;
        int seconds = (T - (hours * 60 * 60) - (minutes * 60));
        std::stringstream time_string;
        time_string << std::setfill('0') << std::setw(2) << hours << ":";
        time_string << std::setfill('0') << std::setw(2) << minutes << ":";
        time_string << std::setfill('0') << std::setw(2) << seconds;
        return time_string.str();
        }
};

namespace xg
{
	int main() {
		xg::sdl				sdl;
		xg::window			window(cfg::screen_size);
		xg::renderer		renderer(window);
        renderer.background_color = xg::color::orange();

        stats	stats;
        //window.hide_cursor();

        try
        {
            {
                xg::music music("resources/example.mp3");
                xg::time		time;
                xg::text text(renderer, cfg::big_font_name, cfg::big_font_size);

                xg::texture texture("resources/star.png", renderer);
                texture.dimensions = v{100, 100};
                texture.blend = xg::blend;

                while (window.new_frame() && music.new_frame())
                {
                    renderer.new_frame();
                    renderer.set_color(xg::color::blue());
                    time.new_frame();

                    stats.render(renderer, time);
                    texture.origin = cfg::screen_size + v{100 * cosf(time.seconds()), 100 * sinf(time.seconds())};
                    texture.render();

                    text.render(renderer, "X Graphics");
                    renderer.present();
                }
            }
        }
        catch (quit&)
        {
        }
        return 0;
    }
}

extern "C" int main(int argc, char *argv[])
	{
	return xg::main();
	}
