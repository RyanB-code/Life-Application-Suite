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

bool ImGuiMods::BeginResizeableChild(const char* title, const float widthMin, const float widthMax, const float height){

	bool success {false};


	static float padding {10};
	static float parentWindowWidth;
	static float childWindowHeight;
	static float childWindowWidth;

	// Algorithm for adjusting child window size
	parentWindowWidth = ImGui::GetWindowContentRegionMax().x;

	childWindowHeight = height;
	childWindowWidth  = widthMax;
	
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

	if(ImGui::BeginChild(title, ImVec2(childWindowWidth, childWindowHeight), true, ImGuiWindowFlags_AlwaysAutoResize)) {
		success = true;
		ImGuiMods::CenterText(title);
	}
	else{
		success = false;
	}


	return success;
}

void ImGuiMods::CenterChildWindow(const float parentWindowWidth, const float childWindowWidth){
	if(parentWindowWidth < childWindowWidth){
		// Do nothing keeping window visible
	}
	else{
		// Make the window be centered
		ImGui::SetCursorPosX((parentWindowWidth - childWindowWidth) * 0.5f);
	}

	return;
}