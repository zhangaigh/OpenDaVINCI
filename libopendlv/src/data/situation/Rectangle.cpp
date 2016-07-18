/**
 * OpenDLV - Simulation environment
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <sstream>
#include <string>

#include <memory>
#include "opendavinci/odcore/base/Deserializer.h"
#include "opendavinci/odcore/base/Hash.h"
#include "opendavinci/odcore/base/SerializationFactory.h"
#include "opendavinci/odcore/base/Serializer.h"
#include "opendlv/data/situation/Rectangle.h"
#include "opendlv/data/situation/Shape.h"
#include "opendlv/data/situation/SituationVisitor.h"
#include "opendlv/data/situation/Vertex3.h"

namespace opendlv {
    namespace data {
        namespace situation {

            using namespace std;
            using namespace odcore::base;

            Rectangle::Rectangle() :
                    m_height(0),
                    m_color(),
                    m_length(0),
                    m_width(0) {
                Shape::setType(Shape::RECTANGLE);
            }

            Rectangle::Rectangle(const Rectangle &obj) :
                    Shape(obj),
                    m_height(obj.m_height),
                    m_color(obj.m_color),
                    m_length(obj.getLength()),
                    m_width(obj.getWidth()) {}

            Rectangle::~Rectangle() {}

            Rectangle& Rectangle::operator=(const Rectangle &obj) {
                Shape::operator=(obj);
                setHeight(obj.getHeight());
                setColor(obj.getColor());
                setLength(obj.getLength());
                setWidth(obj.getWidth());
                return (*this);
            }

            void Rectangle::accept(SituationVisitor &visitor) {
                visitor.visit(*this);
            }

            double Rectangle::getHeight() const {
                return m_height;
            }

            void Rectangle::setHeight(const double &h) {
                m_height = h;
            }

            const Vertex3& Rectangle::getColor() const {
                return m_color;
            }

            void Rectangle::setColor(const Vertex3 &c) {
                m_color = c;
            }

            double Rectangle::getLength() const {
                return m_length;
            }

            void Rectangle::setLength(const double l) {
                m_length = l;
            }

            double Rectangle::getWidth() const {
                return m_width;
            }

            void Rectangle::setWidth(const double w) {
                m_width = w;
            }

            int32_t Rectangle::getID() const {
                return 863;
            }

            const string Rectangle::getShortName() const {
                return "Rectangle";
            }

            const string Rectangle::getLongName() const {
                return "hesperia.data.situation.Rectangle";
            }

            const string Rectangle::toString() const {
                stringstream s;
                s << Shape::toString() << ", Height: " << m_height << ", Color: " << m_color.toString() << ", length: " << m_length << ", width: " << m_width;
                return s.str();
            }

            ostream& Rectangle::operator<<(ostream &out) const {
                // Serializer super class.
                Shape::operator<<(out);

                SerializationFactory& sf=SerializationFactory::getInstance();

                std::shared_ptr<Serializer> s = sf.getQueryableNetstringsSerializer(out);

                s->write(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL6('h', 'e', 'i', 'g', 'h', 't') >::RESULT,
                        m_height);

                s->write(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL5('c', 'o', 'l', 'o', 'r') >::RESULT,
                        m_color);

                s->write(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL6('l', 'e', 'n', 'g', 't', 'h') >::RESULT,
                        m_length);

                s->write(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL5('w', 'i', 'd', 't', 'h') >::RESULT,
                        m_width);

                return out;
            }

            istream& Rectangle::operator>>(istream &in) {
                // Deserializer super class.
                Shape::operator>>(in);

                SerializationFactory& sf=SerializationFactory::getInstance();

                std::shared_ptr<Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

                d->read(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL6('h', 'e', 'i', 'g', 'h', 't') >::RESULT,
                       m_height);

                d->read(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL5('c', 'o', 'l', 'o', 'r') >::RESULT,
                       m_color);

                d->read(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL6('l', 'e', 'n', 'g', 't', 'h') >::RESULT,
                       m_length);

                d->read(CRC32 < OPENDAVINCI_CORE_STRINGLITERAL5('w', 'i', 'd', 't', 'h') >::RESULT,
                       m_width);

                return in;
            }

        }
    }
} // opendlv::data::situation
