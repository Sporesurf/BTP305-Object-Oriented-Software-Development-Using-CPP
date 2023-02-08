// Name: Vlado Tomovic
// Seneca Student ID: 107995151
// Seneca email: vtomovic@myseneca.ca
// Date of completion: 2021/11/26
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include "CustomerOrder.h"

using namespace std;
namespace sdds
{
	size_t CustomerOrder::m_widthField = 0; 

	CustomerOrder::CustomerOrder()
	{
		m_name = "";
		m_product = "";
		m_lstItem = nullptr;
		m_cntItem = 0;
	}

	CustomerOrder::CustomerOrder(const string& n_record)
	{
		Utilities u_obj;
		size_t n_pos = 0;
		bool more = true;

		m_name = u_obj.extractToken(n_record, n_pos, more);
		m_product = u_obj.extractToken(n_record, n_pos, more);

		while (more != false)
		{
			Item* n_item = new Item(u_obj.extractToken(n_record, n_pos, more));
			*this += n_item;
		}

		if (m_widthField < u_obj.getFieldWidth())
		m_widthField = u_obj.getFieldWidth();
	}

	CustomerOrder& CustomerOrder::operator+=(Item* n_item)
	{
		if (n_item != nullptr)
		{
			Item** item = new Item*[m_cntItem + 1];
			for (size_t i = 0; i < m_cntItem; i++)
				item[i] = m_lstItem[i];

			if (m_lstItem != nullptr)
				delete[] m_lstItem;

			item[m_cntItem++] = n_item;
			this->m_lstItem = item;
		}
		else
		{
			throw "ERROR: ARG IS EMPTY";
		}
		return *this;
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& rhs) 
	{ 
		throw "ERROR: NO COPY OPERATIONS";
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& rhs) noexcept 
	{ 
		*this = move(rhs); 
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (m_lstItem != nullptr)
			{
				for (size_t i = 0; i < m_cntItem; i++)
					delete m_lstItem[i];
				delete[] m_lstItem;
			}

			m_name = rhs.m_name;
			m_product = rhs.m_product;
			m_cntItem = rhs.m_cntItem;
			m_lstItem = rhs.m_lstItem;

			rhs.m_name.clear();
			rhs.m_product.clear();
			rhs.m_cntItem = 0;
			rhs.m_lstItem = nullptr;
		}
		return *this;
	}

	bool CustomerOrder::isFilled() const
	{
		for (size_t i = 0; i < m_cntItem; i++)
			if (m_lstItem[i]->m_isFilled == false)
				return false;
		return true;
	}

	bool CustomerOrder::isItemFilled(const string& itemName) const
	{
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == itemName)
				return m_lstItem[i]->m_isFilled;
		}
		return true;
	}

	void CustomerOrder::fillItem(Station& station, ostream& os)
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == station.getItemName())
			{
				if (station.getQuantity() != 0)
				{
					station.updateQuantity();
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[i]->m_isFilled = true;
					os << "    Filled " << m_name
						<< ", " << m_product << " [" << station.getItemName() << "]" << endl;
				}
				else
					os << "    Unable to fill " << m_name << ", " << m_product << " ["
					<< station.getItemName() << "]" << endl;
			}
		}
	}

	void CustomerOrder::display(std::ostream& os) const
	{
		os << m_name << " - " << m_product << endl;

		for (size_t i = 0; i < m_cntItem; i++) 
		{
			os << "[" << setw(u_serialField) << right << setfill('0') 
				<< m_lstItem[i]->m_serialNumber << "] ";

			os << setfill(' ') << left << setw(CustomerOrder::m_widthField);
			os << m_lstItem[i]->m_itemName << " - ";
			
			if (m_lstItem[i]->m_isFilled)
				os << "FILLED";
			else if (!m_lstItem[i]->m_isFilled)
				os << "TO BE FILLED";

			os << endl;
		}
	}

	CustomerOrder::~CustomerOrder()
	{
		for (size_t i = 0; i < m_cntItem; i++)
			delete m_lstItem[i];
		delete[]  m_lstItem;
	}
}