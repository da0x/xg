//
//  xg_main.hpp
//  life
//
//  Created by Daher Alfawares on 1/1/2019.
//  Copyright © 2019 Daher Alfawares. All rights reserved.
//

#include "xg.hpp"

#ifdef main
#undef main
#endif

namespace cfg
{
    const auto screen_size = xg::vec2(840, 160);
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

using xg::v;

class stats
{
public:
    void render(xg::renderer& renderer, const xg::time& time)
        {
        xg::text text(renderer, cfg::big_font_name, cfg::big_font_size);
        xg::grid text_grid = xg::grid().start_at(cfg::screen_size * v{ .5,-5 }).space_by(v{ 80, 0 });
        text.align = xg::bottom | xg::center_h;
        text.origin = text_grid--;
        int T = int(time.seconds());
        int hours = (T) / (60 * 60);
        int minutes = (T - (hours * 60 * 60)) / 60;
        int seconds = (T - (hours * 60 * 60) - (minutes * 60));
        std::stringstream time_string;
        time_string << std::setfill('0') << std::setw(2) << hours << ":";
        time_string << std::setfill('0') << std::setw(2) << minutes << ":";
        time_string << std::setfill('0') << std::setw(2) << seconds;
        text.render(renderer, time_string.str());
        }
};

class note_grid
{
    xg::line line;
    xg::rect rect;
    public:
    std::vector<xg::color> colors;
    std::vector<bool> active = std::vector<bool>(12, false);
    v origin;
    v dimensions;
    void new_frame()
    {
        for (std::size_t i = 0; i < this->active.size(); i++)
        {
            this->active[i] = false;
            this->colors[i] = xg::color::orange();
        }
    }

    void render(xg::renderer& renderer)
    {
        renderer.set_color(cfg::grid_color);
        int grid_size = 1 * 6;
        for (int x = 0; x < 6; x++)
        {
            line.p0 = origin + v{ x,f(x,grid_size).x } *cfg::pitch_grid_block_size;
            line.p1 = origin + v{ x,f(x,grid_size).y } *cfg::pitch_grid_block_size;
            line.render(renderer);
        }
        for (int y = 0; y < 6; y++)
        {
            line.p0 = origin + v{ f(y,grid_size).x, y } *cfg::pitch_grid_block_size;
            line.p1 = origin + v{ f(y,grid_size).y, y } *cfg::pitch_grid_block_size;
            line.render(renderer);
        }
        for (int i = 0; i < 12; i++)
        {
            auto note_on = this->active[i];
            v coordinates;
            switch (i)
            {
                case 0: coordinates = { 2,0 }; break;
                case 1:	coordinates = { 1,1 }; break;
                case 2:	coordinates = { 2,1 }; break;
                case 3:	coordinates = { 3,1 }; break;
                case 4:	coordinates = { 0,2 }; break;
                case 5:	coordinates = { 1,2 }; break;
                case 6:	coordinates = { 3,2 }; break;
                case 7:	coordinates = { 4,2 }; break;
                case 8:	coordinates = { 1,3 }; break;
                case 9:	coordinates = { 2,3 }; break;
                case 10:coordinates = { 3,3 }; break;
                case 11:coordinates = { 2,4 }; break;
            }
            rect.origin = this->origin + coordinates * cfg::pitch_grid_block_size + v{ 1,1 };
            rect.dimensions = cfg::pitch_grid_block_size - v{ 1,1 };
            rect.align = xg::top | xg::left;
            if (note_on)
            {
                renderer.set_color(this->colors[i]);
            }
            else
            {
                renderer.set_color(cfg::block_disabled_color);
            }
            renderer.render(rect);
        }
    }

    v f(int x, int length)
    {
        auto count = length - 1;
        auto d = (x % (length / 2));
        auto ceil = count / 2;
        auto flor = 1 + count / 2;
        if (x < length / 2)
        {
            return v{ ceil - d, flor + d };
        }
        else
        {
            return f(std::abs(x - count), length);
        }
    }
};



namespace xg
{

	int main() {
		xg::sdl				sdl;
		xg::window			window(cfg::screen_size);
		xg::renderer		renderer(window);
        xg::line            line;

        stats	stats;
        //window.hide_cursor();

        try
        {
            std::cout << "waiting...\n";
            while (window.waiting_to_start())
            {
                renderer.new_frame();
                renderer.set_color(xg::color::green());
                line.p0.x = 10;
                line.p0.y = 10;
                line.p1.x = 100;
                line.p1.y = 10;
                line.render(renderer);
                renderer.present();
            }

            std::cout << "starting...\n";

            {
                xg::time		wait_time;
                xg::time		time;
                time.pause = true;
                while (window.new_frame() != -1 && wait_time.seconds() < 5)
                {
                    time.new_frame();
                    wait_time.new_frame();
                    renderer.new_frame();
                    renderer.present();
                }
                xg::music music("example.mp3");
                time.pause = false;
                while (window.new_frame() && music.new_frame())
                {
                    time.new_frame();
                    renderer.new_frame();
                    stats.render(renderer, time);
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
