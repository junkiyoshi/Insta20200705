#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	this->hexagon_height = 16;
	this->hexagon_width = 6;

	for (float x = 36; x <= ofGetWidth() - 36; x += this->hexagon_height * 1.5) {

		for (float y = 36; y <= ofGetHeight() - 36; y += this->hexagon_height * 3) {

			this->number_list.push_back(make_pair(glm::vec2(x, y), 0));
		}
	}

	for (auto& number : this->number_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->number_list) {

			index++;
			if (number == other) { continue; }
			
			if (abs(number.first.x - other.first.x) <= this->hexagon_height * 1.5 && abs(number.first.y - other.first.y) <= this->hexagon_height * 3) {

				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	for (int i = 0; i < 10; i++) {

		this->index_list.push_back((int)ofRandom(this->number_list.size()));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->number_list[next_index].second <= 0) {

				this->number_list[next_index].second = 60;
				this->index_list[i] = next_index;

				break;
			}
		}
	}

	for (int i = 0; i < this->number_list.size(); i++) {

		if (this->number_list[i].second > 0) {
			
			this->number_list[i].second -= 3; 
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (auto& number : this->number_list) {

		if (number.second > 0) {

			int number_index = ofMap(ofNoise(glm::vec3(number.first * 0.05, ofGetFrameNum() * 0.01)), 0, 1, 0, 10);
			this->draw_digital(number.first, number_index, number.second > 30 ? ofColor(39) : ofColor(ofMap(number.second, 0, 30, 239, 39)));
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_digital(glm::vec2 location, int number_index, ofColor color) {

	vector<pair<glm::vec2, float>> part_list = {
		make_pair<glm::vec2, float>(location + glm::vec2(0, -this->hexagon_height), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, 0), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, this->hexagon_height), 90)
	};
	
	vector<vector<int>> index_list = {
		{ 0, 1, 2, 4, 5, 6 },
		{ 2, 5 },
		{ 0, 1, 3, 5, 6 },
		{ 0, 2, 3, 5, 6 },
		{ 2, 3, 4, 5 },
		{ 0, 2, 3, 4, 6 },
		{ 0, 1, 2, 3, 4, 6 },
		{ 0, 2, 5 },
		{ 0, 1, 2, 3, 4, 5, 6 },
		{ 0, 2, 3, 4, 5, 6 },
	};

	for (auto& index : index_list[number_index]) {

		this->draw_hexagon(part_list[index].first, part_list[index].second, color);
	}
}

//--------------------------------------------------------------
void ofApp::draw_hexagon(glm::vec2 location, float deg, ofColor color) {

	ofPushMatrix();
	ofTranslate(location);
	ofRotate(deg);

	vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * 0.5));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * -0.5));

	ofFill();
	ofSetColor(color);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofNoFill();
	ofSetColor(239);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}