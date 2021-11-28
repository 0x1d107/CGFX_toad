// cgfx_lab2.cpp : Defines the entry point for the application.
//

#include "cgfx_lab2.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
using namespace std;
sf::Vector2f operator* (const sf::Vector2f& a, double t) {
	return sf::Vector2f(a.x * t, a.y * t);
}
sf::Vector2f operator* (double t,const sf::Vector2f& a) {
	return sf::Vector2f(a.x * t, a.y * t);
}
sf::Vector2f operator+ (const sf::Vector2f& a,const sf::Vector2f& b) {
	return sf::Vector2f(a.x + b.x, a.y + b.y);
}
sf::Vector2f operator- (const sf::Vector2f& a,const sf::Vector2f& b) {
	return a+(-1.0)*b;
}
sf::Vector2f operator/ (const sf::Vector2f& a, double t) {
	return 1.0/t*a;
}
class BezierCurve {
	std::vector<sf::Vector2f> points;
public:
	BezierCurve(std::vector<sf::Vector2f> points) {
		this->points.assign(points.begin(),points.end());
	}
	sf::Vector2f point(double t) {
		t = 1.0 -t;
		std::vector<sf::Vector2f> pts_old(points);
		std::vector<sf::Vector2f> pts_new;
		while (pts_old.size() > 1) {
			pts_new.resize(pts_old.size() - 1);
			for (int i = 0; i < pts_old.size() - 1; i++) {
				pts_new[i] = pts_old[i] * t + pts_old[i + 1] * (1 - t);
			}
			std::swap(pts_old, pts_new);
		}
		if(!pts_old.size())
			return sf::Vector2f{0,0};
		return pts_old[0];

	}
};
class BCurve3{
	std::vector<sf::Vector2f> vec;
	BezierCurve* curve;
	public:
	BCurve3(sf::Vector2f a,sf::Vector2f b,sf::Vector2f c,sf::Vector2f d){
		vec.clear();
		vec.push_back(a);
		vec.push_back(b);
		vec.push_back(c);
		vec.push_back(d);
		curve = new BezierCurve(vec);
	}
	void toVertexArray(sf::VertexArray &va){
		auto from =  curve->point(0.0);
		auto to = curve->point(1.0);
		std::cout<<"("<<from.x<<','<<from.y<<") -- ("<<to.x<<','<<to.y<<")"<<std::endl;
		for(double t=0.0;t<=1.0; t+=0.01){
			va.append(sf::Vertex{curve->point(t),sf::Color::White});

		}

	}
	static std::vector<BCurve3 *> getCurvesForPoints(const std::vector<sf::Vector2f>& points){
		std::vector<BCurve3 *> curves;
		for(auto it = points.begin();it!=points.end()&&it+1!=points.end();it++){
			auto a = *it;
			auto b = *(it+1);
			sf::Vector2f p0,p3;
			if(it == points.begin()){
				p0 = a;
			}else{
				p0 = *(it-1);
			}
			if(it+2 == points.end()){
				p3 = b;
			}else{
				p3 = *(it+2);
			}
			std::cout<<"a: "<< a.x <<',' << a.y<<std::endl;
			std::cout <<"b: "<<b.x <<','<<b.y<< std::endl;
			curves.push_back(new BCurve3(a,a+(b-p0)/6,b-(p3-a)/6,b));
		}


		return curves;

	}
	static void drawCurves(const std::vector<sf::Vector2f>& points, sf::VertexArray &va ){
		for(auto bc: getCurvesForPoints(points) ){
			bc->toVertexArray(va);
			delete bc;
		}

	}
	~BCurve3(){
		delete curve;
	}
};
int main()
{
	
	sf::VertexArray vertexarray(sf::LineStrip);
	int n;
	std::cin >> n;
	std::cout << n <<std::endl;
	std::vector<sf::Vector2f> points;
	for (int i = 0; i < n; i++) {
		float x,y;
		std::cin >> x;
		std::cin >> y;
		std::cout<<x <<' '<<y<<std::endl;
		points.push_back(sf::Vector2f{ x,y });
	}
	sf::RenderWindow win(sf::VideoMode(800, 600), "CGFX lab2");
	BCurve3::drawCurves(points,vertexarray);

	while(win.isOpen()){
		sf::Event event;
		while(win.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				win.close();
			}
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				points.push_back(sf::Vector2f{(float)event.mouseButton.x,(float)event.mouseButton.y});
				vertexarray.clear();
				BCurve3::drawCurves(points,vertexarray);
			}
		}
		win.clear();
		win.draw(vertexarray);
		for(int i=0;i<points.size();i++){
			sf::Vector2f pos = points[i];
			sf::CircleShape node;
			node.setPosition(pos - sf::Vector2f{2,2});
			node.setRadius(2.0);
			node.setFillColor(sf::Color::White);
			win.draw(node);
		}

		win.display();

		
	}
	return 0;
}
