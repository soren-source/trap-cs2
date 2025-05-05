#include "gui.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include <ui/blur/impl.hpp>
#include <ui/ui.hpp>
#include <backend/keybinding/KeyHandler.hpp>

Gui::Gui( )
{
    std::function<void()> handleKeyInput = [ & ] ( ) {
        this->m_IsOpened ^= 1;
        ImGui::GetIO( ).MouseDrawCursor = this->m_IsOpened;
        };

    const auto& menuBind = Keybind( VK_INSERT, handleKeyInput );
    ADD_KEYBIND( menuBind );
}

auto Gui::Render( ) -> void
{
	if ( !this->m_IsOpened )
		return;

    ImGui::SetNextWindowSize( ui::size );
    ImGui::Begin( "weave", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground ); {
        ImGui::BeginChild( "nav", { 176, ImGui::GetWindowHeight( ) }, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar ); {
            ImGui::BeginChild( "brand", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, ImGui::GetColorU32( ImGuiCol_ChildBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersTop );
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) }, ImGui::GetColorU32( ImGuiCol_Scheme ) );
                ImGui::GetWindowDrawList( )->AddImage( this->m_Logo, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) / 2 - 31 / 2, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 14 / 2 - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) / 2 + 31 / 2, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 + 14 / 2 - 2 }, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ) );
            }
            ImGui::EndChild( );

            ImGui::SetCursorPosY( 42 );
            ImGui::BeginChild( "tabs", { ImGui::GetWindowWidth( ), ImGui::GetWindowHeight( ) - 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                imgui_blur::create_blur( ImGui::GetWindowDrawList( ), ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_WindowBg, 0.875f ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );

                ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 1, 1 } );
                ImGui::BeginGroup( ); {
                    for ( int i = 0; i < ui::tabs.size( ); ++i ) {
                        ui::tab( i );
                    }
                }
                ImGui::EndGroup( );
                ImGui::PopStyleVar( );
            }
            ImGui::EndChild( );

            ImGui::SetCursorPos( { 0, ImGui::GetWindowHeight( ) - 42 } );
            ImGui::BeginChild( "profile", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + 1 }, ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_Header ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );
                ImGui::GetWindowDrawList( )->AddLine( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y }, ImGui::GetColorU32( ImGuiCol_BorderShadow ) );
                ImGui::GetWindowDrawList( )->AddImageRounded( this->m_Avatar, { ImGui::GetWindowPos( ).x + 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 24 / 2 }, { ImGui::GetWindowPos( ).x + 39, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 + 24 / 2 }, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ), 100 );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 47, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "evrope" ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Text ), "evrope" );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "28d" ).x - 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "28d" ).y / 2 }, ImGui::GetColorU32( ImGuiCol_TextDisabled ), "28d" );
            }
            ImGui::EndChild( );
        }
        ImGui::EndChild( );

        ImGui::SetCursorPos( { 185, 0 } );
        ImGui::BeginChild( "main", ImGui::GetContentRegionAvail( ), 0, ImGuiWindowFlags_NoBackground ); {
            ImGui::BeginChild( "main header", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, ImGui::GetColorU32( ImGuiCol_ChildBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersTop );
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) }, ImGui::GetColorU32( ImGuiCol_Scheme ) );
                ImGui::GetWindowDrawList( )->AddText( { ImGui::GetWindowPos( ).x + 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - ImGui::CalcTextSize( ui::tabs[ ui::cur_tab ].m_icon, 0, 1 ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Text, ui::anim ), ui::tabs[ ui::cur_tab ].m_icon, ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_icon ) );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 40, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).y / 2 }, ImGui::GetColorU32( ui::tabs[ ui::cur_tab ].m_subtabs.size( ) > 0 ? ImGuiCol_TextDisabled : ImGuiCol_Text, ui::anim ), ui::tabs[ ui::cur_tab ].m_name, ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) );
                if ( ui::tabs[ ui::cur_tab ].m_subtabs.size( ) > 0 ) {
                    ImGui::RenderArrow( ImGui::GetWindowDrawList( ), { ImGui::GetWindowPos( ).x + 47 + font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 10 }, ImGui::GetColorU32( ImGuiCol_Text, ui::anim * ui::content_anim ), ImGuiDir_Right, 12 );
                    ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 60 + font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ], FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ] ) ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Scheme, ui::anim * ui::content_anim ), ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ], ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ] ) );
                }
            }
            ImGui::EndChild( );

            ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + 42 }, ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_WindowBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );

            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 10, 10 } );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 15, 15 } );
            ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ui::anim * ui::content_anim );
            ImGui::SetCursorPosY( 42 );
            ImGui::BeginChild( "content", ImGui::GetContentRegionAvail( ), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysUseWindowPadding ); {
                ui::render_page( );
            }
            ImGui::EndChild( );
            ImGui::PopStyleVar( 3 );
        }
        ImGui::EndChild( );
    }
    ImGui::End( );

    ui::handle_anims( );
}