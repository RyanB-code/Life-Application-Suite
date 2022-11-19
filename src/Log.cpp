#include "Log.h"

#pragma warning(disable : 4996)

// static variable initialization
std::vector<Log> Log::s_logList{}; // Initializer for s_logList to be used
std::string Log::m_path = "";
bool Log::m_wroteAllStoredLogs = false;

Log::Log(LogCode code, std::string msg)
	: m_code	{ code }, 
	m_msg		{ msg }
{
	s_logList.push_back(*this);

	// Check if the log list will write to console or the file
	if (m_path == "") {
		std::cout << *this;
	}
	else {
		// Write to file
		if(std::filesystem::exists(m_path)) {
			std::ofstream file{ m_path, std::ios_base::app};

			// Check if stored logs have already been written to the file. If not, writes them
			if(!m_wroteAllStoredLogs){
				for(Log currentLog : s_logList){
					file << currentLog;
				}
				m_wroteAllStoredLogs = true;
			}
			else{		
				file << *this;	// If stored logs have been written already, append file normally
			}
		}
		else {
			// Write to console if issue finding file
			std::cout << *this;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Log& log) {

	tm* ltm = localtime(&log.m_timestamp);

	//Time in HH:MM:SS format
	os << "["
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec << "]";

	os << "\t[";
	switch (log.m_code) {
	case LogCode::FATAL:
		os << "FATAL";
		break;
	case LogCode::WARNING:
		os << "WARNING";
		break;
	case LogCode::ROUTINE:
		os << "ROUTINE";
		break;
	case LogCode::LOG:
		os << "  LOG  ";
		break;
	}
	os << "]";

	os << "\t" << log.m_msg << '\n';

	return os;
}



void Debug::Display(){
	 if(ImGui::Begin(m_name.c_str(), &m_shown, 0)){

        static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

		static bool AutoScroll { true };
		static bool ScrollToBottom { true };

		ImGuiMods::Header("Debug Logs", "Shows all Debugs Logged for this instance");
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::SameLine();


		if(ImGui::Button("Add Log", ImVec2(100, TEXT_BASE_HEIGHT))){
			static int addLogCounter{0};
			++addLogCounter;
			std::ostringstream os; os << "Test Log " << addLogCounter;
			Log(LogCode::LOG, os.str());
		}


		ImGui::Spacing();

		// Log Region
		 if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar))
        {

			for(Log& log : Log::s_logList){
				std::ostringstream os{}; os << log;
				std::string text {os.str()};

				ImGui::TextUnformatted(text.c_str());
			}

			if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

			
		}
		ImGui::EndChild();

		/*
		ImGui::Spacing();
		if(ImGui::BeginTable("Repairs", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
		{
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Code", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableHeadersRow();

		
			for(Log& log : Log::s_logList){
				// Log Buffers
				tm* ltm = localtime(&log.m_timestamp);
				std::ostringstream os;

				os  << std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
					<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
					<< std::setw(2) << std::setfill('0') << ltm->tm_sec;
				std::string timestamp{os.str()};

				os.str("");

				switch (log.m_code) {
					case LogCode::FATAL:
						os << "FATAL";
						break;
					case LogCode::WARNING:
						os << "WARNING";
						break;
					case LogCode::ROUTINE:
						os << "ROUTINE";
						break;
					case LogCode::LOG:
						os << "LOG";
						break;
				}

				std::string code{os.str()};

				ImGui::TableNextRow();
				int column{0};

				ImGui::TableSetColumnIndex(column);	
				ImGui::Text("%s", timestamp.c_str());
				++column;

				ImGui::TableSetColumnIndex(column);	
				ImGui::Text("%s", code.c_str());
				++column;

				ImGui::TableSetColumnIndex(column);	
				ImGui::Text("%s", log.m_msg.c_str());
				++column;

			}

			ImGui::EndTable();
		}
		*/
	 
	 }


	ImGui::End(); //End Window

	return;
}

