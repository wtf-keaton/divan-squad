#include "menu.hh"
#include "../variables.hh"

void menu::draw()
{
	ImGui::Begin( _( "divan" ), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar );
	{
		auto draw = ImGui::GetWindowDrawList();
		auto p = ImGui::GetWindowPos();
		static auto page = 0;
		ImGui::SetWindowSize( ImVec2( 430, 470 ) );

		draw->AddRectFilled( p, p + ImVec2( 430, 470 ), IM_COL32( 33, 33, 33, 255 ), 8.f );
		draw->AddRectFilled( p, p + ImVec2( 430, 37 ), IM_COL32( 25, 25, 25, 255 ), 8.f, ImDrawCornerFlags_Top );

		draw->AddLine( p + ImVec2( 130, 7 ), p + ImVec2( 130, 32 ), IM_COL32( 100, 100, 100, 200 ) );
		draw->AddLine( p + ImVec2( 0, 37 ), p + ImVec2( 430, 37 ), IM_COL32( 100, 100, 100, 255 ) );

		ImGui::SetCursorPos( ImVec2( 8, 3 ) );
		ImGui::PushFont( icons );
		if ( custom->tab( _( "0" ), page == 0, ImVec2( 32, 32 ) ) ) page = 0;
		ImGui::SameLine();
		if ( custom->tab( _( "1" ), page == 1, ImVec2( 32, 32 ) ) ) page = 1;
		ImGui::SameLine();
		if ( custom->tab( _( "2" ), page == 2, ImVec2( 32, 32 ) ) ) page = 2; 
		ImGui::PopFont();

		ImGui::BeginGroup();
		switch ( page )
		{
		case 0:
			ImGui::SetCursorPos( ImVec2( 13, 53 ) );

			ImGui::BeginChild( _( "##main" ), ImVec2( 405, 400 ), true );
			{
				ImGui::SetCursorPosX( 8 );
				ImGui::BeginGroup();
				ImGui::Checkbox( _( "Enable" ), &vars::aimbot::enable );
				if ( ImGui::IsItemHovered( ) )
				{
					ImGui::SetTooltip( _( "Aimbot work on \"Right Mouse Button\"") );
				}
				ImGui::Checkbox( _( "Draw aim bone" ), &vars::aimbot::draw_aimbone );
				ImGui::Checkbox( _( "Draw fov" ), &vars::aimbot::draw_fov );
				ImGui::SliderInt( _( "FOV" ), &vars::aimbot::FOV, 1, 180 );
				ImGui::SliderInt( _( "Smooth" ), &vars::aimbot::smooth, 1, 15 );
				ImGui::EndGroup();
			}
			ImGui::EndChild();
			break;
		case 1:
		{
			static auto subtab = 0;

			ImGui::SetCursorPos( ImVec2( 140, 3 ) );
			ImGui::BeginGroup();
			{
				if ( custom->subtab( _( "Players" ), subtab == 0, ImVec2( 0, 32 ) ) ) subtab = 0;
				ImGui::SameLine( 0, 0 );
				if ( custom->subtab( _( "Transport" ), subtab == 1, ImVec2( 0, 32 ) ) ) subtab = 1;
				ImGui::SameLine( 0, 0 );
				if ( custom->subtab( _( "World" ), subtab == 2, ImVec2( 0, 32 ) ) ) subtab = 2;
			}
			ImGui::EndGroup();


			ImGui::SetCursorPos( ImVec2( 13, 53 ) );
			ImGui::BeginGroup();
			ImGui::BeginChild( _( "##main" ), ImVec2( 405, 400 ), true );
			{
				ImGui::SetCursorPosX( 8 );
				ImGui::BeginGroup();
				switch ( subtab )
				{
				case 0:

					ImGui::Checkbox( _( "Enable Players" ), &vars::visuals::players::enable );
					ImGui::Checkbox( _( "Disable Team" ), &vars::visuals::players::disable_team );
					ImGui::SliderInt( _( "Drawing Distance" ), &vars::visuals::players::distance, 0, 5000, _( "%dm" ) );
					
					ImGui::Checkbox( _( "Box" ), &vars::visuals::players::box );
					ImGui::SameLine();
					ImGui::ColorEdit4( _( "##box_color" ), vars::visuals::players::box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview );

					if ( vars::visuals::players::box )
						ImGui::Checkbox( _( "Outline box" ), &vars::visuals::players::box_outline );

					ImGui::Checkbox( _( "Name ESP" ), &vars::visuals::players::names );
					ImGui::SameLine();
					ImGui::ColorEdit4( _( "##name_color" ), vars::visuals::players::name_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview );
					ImGui::Checkbox( _( "Skelet ESP" ), &vars::visuals::players::skelet );
					ImGui::SameLine();
					ImGui::ColorEdit4( _( "##skelet_color" ), vars::visuals::players::skelet_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview );
					ImGui::Checkbox( _( "Snapline" ), &vars::visuals::players::snapline );

					break;
				case 1:
					ImGui::Checkbox( _( "Enable Transport" ), &vars::visuals::transport::enable );
					ImGui::SliderInt( _( "Drawing Distance" ), &vars::visuals::transport::distance, 0, 5000, _( "%dm" ) );

					custom->checkbox( _( "Kamaz" ), &vars::visuals::transport::kamaz, _( "##Kamaz" ), vars::visuals::transport::kamaz_color );
					custom->checkbox( _( "Minsk" ), &vars::visuals::transport::minsk, _( "##minsk" ), vars::visuals::transport::minsk_color );
					custom->checkbox( _( "Technical Pickup" ), &vars::visuals::transport::tech_pickup, _( "##Technical" ), vars::visuals::transport::tech_color );
					custom->checkbox( _( "Jeep" ), &vars::visuals::transport::jeep, _( "##Jeep" ), vars::visuals::transport::jeep_color );
					custom->checkbox( _( "LUV-A1" ), &vars::visuals::transport::luv, _( "##luv" ), vars::visuals::transport::luv_color );
					custom->checkbox( _( "M939" ), &vars::visuals::transport::m939, _( "##m939" ), vars::visuals::transport::m939_color );
					custom->checkbox( _( "MAN" ), &vars::visuals::transport::man, _( "##man" ), vars::visuals::transport::man_color );
					custom->checkbox( _( "Ural 375D" ), &vars::visuals::transport::ural375D, _( "##ural375D" ), vars::visuals::transport::ural375D_color );
					custom->checkbox( _( "Ural 4320" ), &vars::visuals::transport::ural4320, _( "##ural4320" ), vars::visuals::transport::ural4320_color );
					custom->checkbox( _( "M-ATV" ), &vars::visuals::transport::matv, _( "##matv" ), vars::visuals::transport::matv_color );
					custom->checkbox( _( "Tigr-M" ), &vars::visuals::transport::tigrm, _( "##tigrm" ), vars::visuals::transport::tigrm_color );
					custom->checkbox( _( "TAPV" ), &vars::visuals::transport::tapv, _( "##tapv" ), vars::visuals::transport::tapv_color );
					custom->checkbox( _( "BRDM-2" ), &vars::visuals::transport::brdm, _( "##brdm" ), vars::visuals::transport::brdm_color );
					custom->checkbox( _( "BTR-80" ), &vars::visuals::transport::btr80, _( "##btr80" ), vars::visuals::transport::btr80_color );
					custom->checkbox( _( "BTR-82A" ), &vars::visuals::transport::btr82a, _( "##btr82a" ), vars::visuals::transport::btr82a_color );
					custom->checkbox( _( "LAV 6" ), &vars::visuals::transport::lav6, _( "##lav6" ), vars::visuals::transport::lav6_color );
					custom->checkbox( _( "LAV 3" ), &vars::visuals::transport::lav3, _( "##lav3" ), vars::visuals::transport::lav3_color );
					custom->checkbox( _( "M113A3" ), &vars::visuals::transport::m113a3, _( "##m113a3" ), vars::visuals::transport::m113a3_color );
					custom->checkbox( _( "Ocelot" ), &vars::visuals::transport::lppv, _( "##lppv" ), vars::visuals::transport::lppv_color );
					custom->checkbox( _( "M1126" ), &vars::visuals::transport::m1126, _( "##m1126" ), vars::visuals::transport::m1126_color );
					custom->checkbox( _( "FV510" ), &vars::visuals::transport::fv510, _( "##fv510" ), vars::visuals::transport::fv510_color );
					custom->checkbox( _( "FV432" ), &vars::visuals::transport::fv432, _( "##FV432" ), vars::visuals::transport::fv432_color );
					custom->checkbox( _( "FV4034" ), &vars::visuals::transport::fv4034, _( "##fv4034" ), vars::visuals::transport::fv4034_color );
					custom->checkbox( _( "FV107" ), &vars::visuals::transport::fv107, _( "##fv107" ), vars::visuals::transport::fv107_color );
					custom->checkbox( _( "MT-LB" ), &vars::visuals::transport::mtlb, _( "##mtlb" ), vars::visuals::transport::mtlb_color );
					custom->checkbox( _( "BMP-1" ), &vars::visuals::transport::bmp1, _( "##bmp1" ), vars::visuals::transport::bmp1_color );
					custom->checkbox( _( "BMP-2" ), &vars::visuals::transport::bmp2, _( "##bmp2" ), vars::visuals::transport::bmp2_color );
					custom->checkbox( _( "M2A3" ), &vars::visuals::transport::m2a3, _( "##m2a3" ), vars::visuals::transport::m2a3_color );
					custom->checkbox( _( "MSVS" ), &vars::visuals::transport::msvs, _( "##msvs" ), vars::visuals::transport::msvs_color );
					custom->checkbox( _( "M1A2" ), &vars::visuals::transport::m1a2, _( "##m1a2" ), vars::visuals::transport::m1a2_color );
					custom->checkbox( _( "T-62" ), &vars::visuals::transport::t62, _( "##t62" ), vars::visuals::transport::t62_color );
					custom->checkbox( _( "T-72" ), &vars::visuals::transport::t72b3, _( "##T72" ), vars::visuals::transport::t72b3_color );
					custom->checkbox( _( "Mi-8" ), &vars::visuals::transport::mi8, _( "##mi8" ), vars::visuals::transport::mi8_color );
					custom->checkbox( _( "UH-60M" ), &vars::visuals::transport::uh60, _( "##uh60" ), vars::visuals::transport::uh60_color );
					custom->checkbox( _( "SA330" ), &vars::visuals::transport::sa330, _( "##sa330" ), vars::visuals::transport::sa330_color );
					break;
				case 2:
					ImGui::Checkbox( _( "Enable World" ), &vars::visuals::world::enable );

					custom->checkbox( _( "Ammocrates" ), &vars::visuals::world::ammocrate, _( "##Ammocrates1" ), vars::visuals::world::ammocrate_color );

					ImGui::SliderInt( _( "Drawing Distance" ), &vars::visuals::world::distance, 0, 500, _( "%dm" ) );

					break;
				}
				ImGui::EndGroup();

			}
			ImGui::EndChild();
			ImGui::EndGroup();
			break;

		}
		case 2:
			ImGui::SetCursorPos( ImVec2( 13, 53 ) );
			ImGui::BeginGroup();
			ImGui::BeginChild( _( "##main" ), ImVec2( 405, 400 ), true );
			{
				ImGui::SetCursorPosX( 8 );
				ImGui::BeginGroup();
				ImGui::Checkbox( _( "Enable" ), &vars::misc::enable );
				ImGui::Checkbox( _( "Speed hack" ), &vars::misc::speed_hack );
				//ImGui::Checkbox( _( "Debug" ), &vars::misc::debug );
				if ( vars::misc::speed_hack )
					ImGui::SliderInt( _( "Speed" ), &vars::misc::speed_value, 0, 350 );
			}
			ImGui::EndGroup();
			break;

		}
	}
	ImGui::End();
}