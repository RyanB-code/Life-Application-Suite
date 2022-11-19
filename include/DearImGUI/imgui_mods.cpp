#include "imgui_mods.h"

void ImGuiMods::HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	return;
}

void ImGuiMods::CenterText(const char* text){

	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth   = ImGui::CalcTextSize(text).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text);
	return; 
}

void ImGuiMods::Header(const char* title, const char* text){
	ImGuiMods::CenterText(title);

	if(!text) {	/* Do Nothing */ }
	else{
		ImGui::TextWrapped(text);
	}
	return;
}

void ImGuiMods::BeginResizeableChild(const char* title, const float widthMin, const float widthMax, const float height){

	static float padding {10};

	// Algorithm for adjusting child window size
	static float parentWindowWidth;
	parentWindowWidth = ImGui::GetWindowContentRegionMax().x;

	static float childWindowHeight = height;
	static float childWindowWidth {widthMax};
	
	if(parentWindowWidth > (widthMax + padding) ){
		// Do nothing therefore keep window at widthMax pixels if the parent window is larger than specified
	}
	else{
		if(parentWindowWidth < (widthMin + padding)){
			// If parent window size is smaller than widthMin + padding, keep at min width
			childWindowWidth  = widthMin;
		}
		else{
			childWindowWidth = parentWindowWidth - 10;	// If smaller than max size, rersize to the window size minus 10 pixels
		}
	}

	ImGui::BeginChild(title, ImVec2(childWindowWidth, childWindowHeight), true, ImGuiWindowFlags_AlwaysAutoResize);
	ImGuiMods::CenterText(title);

	return;
}